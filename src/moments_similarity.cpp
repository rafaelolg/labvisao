#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "moments_similarity.h"

#define COLS 	3
#define ROWS 	3
#define RECTS 	ROWS*COLS

// Weights of importance of each method for calculating the similarity
#define RECTANGLES_WEIGHT 		1.0
#define CENTRAL_WINDOW_WEIGHT	0.4
#define TOTAL_WEIGHT			RECTANGLES_WEIGHT + CENTRAL_WINDOW_WEIGHT 

#define CENTRAL_SIZE_RATIO		0.5


double LocalizedMomentsSimilarityCalculator::calculate(Mat a, Mat b){
	// Equalize channels
	Mat aEq = equalizedMat(a);
	Mat bEq = equalizedMat(b);

	// Possible result could be a combination of 3 methods
	// - evaluate NxN rectangles of Mat a with the NxN rectangles of B
	// - evaluate the central rectangle
	// - use sliding windows
	double rectanglesSimilarity = RECTANGLES_WEIGHT != 0 ?
			rectangleCalculate(aEq, bEq) : 0.0;
	double centralWindowSimilarity = CENTRAL_WINDOW_WEIGHT != 0 ?
			centralWindowCalculate(aEq, bEq) : 0.0;
	// printf("%f, %f, %f\n", rectanglesSimilarity, slidingWindowSimilarity, centralWindowSimilarity);
	// Return avg
	double sum = 0;
	sum += RECTANGLES_WEIGHT * rectanglesSimilarity;
	sum += CENTRAL_WINDOW_WEIGHT * centralWindowSimilarity;

	return sum/TOTAL_WEIGHT;
}

/**
 * Compare NxN rectangles of an image
 * with NxN rectangles of the second image
 */
double LocalizedMomentsSimilarityCalculator::rectangleCalculate(Mat a,Mat b){
	// similarity vector of each rectangle
	vector<double> similarities(RECTS);

	for (int i = 0; i < RECTS; i++){
		Mat aRect = rectangleForIndex(a, i);
		Mat bRect = rectangleForIndex(b, i);
		similarities[i] = similarityBetween(aRect, bRect);
	}

	// Calculate the similarity sum
	double sum = 0;
	for (int i = 0; i < RECTS; i++){
		sum += similarities[i];
	}

	return sum/similarities.size();
}

/**
 * Compare the similarity between the central windows of those images
 */
double LocalizedMomentsSimilarityCalculator::centralWindowCalculate(Mat a,Mat b){
	Mat aRect = centralRectangle(a);
	Mat bRect = centralRectangle(b);
	return similarityBetween(aRect, bRect);
}


double LocalizedMomentsSimilarityCalculator::similarityBetween(Mat a,Mat b){
    Scalar a_stddev, b_stddev, a_mean, b_mean;
    meanStdDev(a, a_mean, a_stddev);
    meanStdDev(b, b_mean, b_stddev);
    double distance_means = norm(a_mean - b_mean);
    double distance_stddevs = norm(a_stddev - b_stddev);
    return 1 / (1 + distance_means + distance_stddevs);
}

/**
 *
 */
Mat LocalizedMomentsSimilarityCalculator::rectangleForIndex(Mat mat,int index){
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

Mat LocalizedMomentsSimilarityCalculator::centralRectangle(Mat mat){
	double ratio = CENTRAL_SIZE_RATIO;
	int width = (int)(mat.cols*ratio);
	int height = (int)(mat.rows*ratio);

	int x0 = mat.cols/2 - width/2;
	int x1 = mat.cols/2 + width/2;
	int y0 = mat.rows/2 - height/2;
	int y1 = mat.rows/2 + height/2;

	return cloneMat(mat, x0, y0, x1, y1);
}

Mat LocalizedMomentsSimilarityCalculator::cloneMat(Mat src, int x0, int y0, int x1, int y1){
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

/**
 * Equalize the colors in order to level all the pictures' colors
 * around the same level.
 */
Mat LocalizedMomentsSimilarityCalculator::equalizedMat(Mat mat){
	// equalizeHist() can only be applied to a grayscale image
	// Since our picture is a BGR one, split it into 3 channels
	// and equalize each of them separatedly and merge the output
	// Not the best solution, but simple enough to perform
	vector<Mat> channels(3);
	split(mat, channels);

	// Equalize channels
	vector<Mat> channelsEq(3);
	equalizeHist(channels[0], channelsEq[0]);
	equalizeHist(channels[1], channelsEq[1]);
	equalizeHist(channels[2], channelsEq[2]);

	Mat matEq;
	merge(channelsEq, matEq);
	return matEq;
}

