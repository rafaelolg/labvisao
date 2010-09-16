#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "localized_similarity.h"

#define COLS 	3
#define ROWS 	3
#define RECTS 	ROWS*COLS

// Weights of importance of each method for calculating the distance
#define RECTANGLES_WEIGHT 		1.0
#define CENTRAL_WINDOW_WEIGHT	1.0
#define SLIDING_WINDOW_WEIGHT 	0.0
#define TOTAL_WEIGHT			RECTANGLES_WEIGHT + CENTRAL_WINDOW_WEIGHT + SLIDING_WINDOW_WEIGHT

#define CENTRAL_SIZE_RATIO		0.5

#define SLIDING_WINDOW_PASS			3
#define SLIDING_WINDOW_SIZE_RATIO	0.5

double LocalizedSimilarityCalculator::calculate(Mat a, Mat b){
	// Possible result could be a combination of 3 methods
	// - evaluate NxN rectangles of Mat a with the NxN rectangles of B
	// - evaluate the central rectangle
	// - use sliding windows
	double rectanglesDistance = RECTANGLES_WEIGHT != 0 ?
			rectangleCalculate(a, b) : 0.0;
	double slidingWindowDistance = SLIDING_WINDOW_WEIGHT != 0 ?
			slidingWindowCalculate(a, b) : 0.0;
	double centralWindowDistance = CENTRAL_WINDOW_WEIGHT != 0 ?
			centralWindowCalculate(a, b) : 0.0;
	// printf("%f, %f, %f\n", rectanglesDistance, slidingWindowDistance, centralWindowDistance);
	// Return avg
	double sum = 0;
	sum += RECTANGLES_WEIGHT * rectanglesDistance;
	sum += SLIDING_WINDOW_WEIGHT * slidingWindowDistance;
	sum += CENTRAL_WINDOW_WEIGHT * centralWindowDistance;

	return sum/TOTAL_WEIGHT;
}

/**
 * Compare NxN rectangles of an image
 * with NxN rectangles of the second image
 */
double LocalizedSimilarityCalculator::rectangleCalculate(Mat a,Mat b){
	// distance vector of each rectangle
	vector<double> distances(RECTS);

	for (int i = 0; i < RECTS; i++){
		Mat aRect = rectangleForIndex(a, i);
		Mat bRect = rectangleForIndex(b, i);
		distances[i] = distanceBetween(aRect, bRect);
	}

	// Calculate the distance sum
	double sum = 0;
	for (int i = 0; i < RECTS; i++){
		sum += distances[i];
	}

	return sum/distances.size();
}

/**
 * Compare the distance between the central windows of those images
 */
double LocalizedSimilarityCalculator::centralWindowCalculate(Mat a,Mat b){
	Mat aRect = centralRectangle(a);
	Mat bRect = centralRectangle(b);
	return distanceBetween(aRect, bRect);
}

/**
 * Use slide windows in order to find which window has the smallest
 * distance to the first picture
 */
double LocalizedSimilarityCalculator::slidingWindowCalculate(Mat a,Mat b){
	// Rather than using the first image directly, use its center only
	Mat templ = centralRectangle(a);
	double best = 1.0;
	double ratio = SLIDING_WINDOW_SIZE_RATIO;
	int numPass = SLIDING_WINDOW_PASS;

	// Slide 9 times, with a window 4 times smaller than the image
	int colPass = b.cols*(int)((1.0-1.0*ratio)/numPass);
	int rowPass = b.rows*(int)((1.0-1.0*ratio)/numPass);
	int windowWidth = (int)(b.cols*ratio);
	int windowHeight = (int)(b.rows*ratio);

	// Slide window and compare with the template
	// Pick the window with the smallest distance
	for (int j = 0 ; j < numPass ; j++){
		for (int i = 0 ; i < numPass ; i++){
			int x0 = i*colPass;
			int y0 = j*rowPass;
			int x1 = x0+windowWidth;
			int y1 = y0+windowHeight;

			// Compare window to the template of the first picture
			Mat window = cloneMat(b, x0, y0, x1, y1);
			double d = distanceBetween(templ, window);
			if (d < best){
				best = d;
			}
		}
	}

	return best;
}

double LocalizedSimilarityCalculator::distanceBetween(Mat a,Mat b){
	// TODO do something to calculate the distances
	// For test purposes, it uses the very same code for
	// HistogramSimilarity
	MatND hista, histb;
	Mat a_hsv, b_hsv;

	int histsize[] = { 64, 64 };
	int channels[] = { 0, 1 };
	float hrange[] = { 0, 180 };
	float srange[] = { 0, 255 };
	const float *ranges[] = { hrange, srange };

	cvtColor(a, a_hsv, CV_BGR2HSV);
	cvtColor(b, b_hsv, CV_BGR2HSV);

	calcHist(&a_hsv, 1, channels, Mat(), hista, 2, histsize, ranges, true, false);
	calcHist(&b_hsv, 1, channels, Mat(), histb, 2, histsize, ranges, true, false);
	double similarity = compareHist(hista, histb, CV_COMP_INTERSECT) /((a.rows-1) * (a.cols-1));
	return similarity;
}

/**
 *
 */
Mat LocalizedSimilarityCalculator::rectangleForIndex(Mat mat,int index){
	// Find rectangle coordinates
	if (index < RECTS){
		int col = index%COLS;
		int row = index/COLS;

		int x0 = col*mat.cols/COLS;
		int x1 = (col+1)*mat.cols/COLS;
		int y0 = row*mat.rows/ROWS;
		int y1 = (row+1)*mat.rows/ROWS;

		return cloneMat(mat, x0, y0, x1, y1);
	} else {
		// Invalid index
		return Mat_<Vec3b>(1,1);
	}
}

Mat LocalizedSimilarityCalculator::centralRectangle(Mat mat){
	double ratio = CENTRAL_SIZE_RATIO;
	int width = (int)(mat.cols*ratio);
	int height = (int)(mat.rows*ratio);

	int x0 = mat.cols/2 - width/2;
	int x1 = mat.cols/2 + width/2;
	int y0 = mat.rows/2 - height/2;
	int y1 = mat.rows/2 + height/2;

	return cloneMat(mat, x0, y0, x1, y1);
}

Mat LocalizedSimilarityCalculator::cloneMat(Mat src, int x0, int y0, int x1, int y1){
	// It's important to note that the src Mat has to be a 3-channel
	// coloured img. Images from imread() with flags=1 will always be
	// acceptable
	Mat_<Vec3b> rect(y1-y0, x1-x0);
	for (int y = y0; y < y1 ; y++){
		for (int x = x0; x < x1 ; x++){
			Vec3b pixel = src.at<Vec3b>(y,x);
			rect.at<Vec3b>(y-y0,x-x0) = pixel;
		}
	}
	return rect;
}
