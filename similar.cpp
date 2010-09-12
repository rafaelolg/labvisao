#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

#include "similar.h"
#include "surf.h"

using namespace std;
using namespace cv;

double HistogramDistanceCalculator::calculate(Mat a, Mat b)
{
	MatND hista, histb;
	Mat a_hsv, b_hsv;

	int histsize[] = { 180, 256 };
	int channels[] = { 0, 1 };
	float hrange[] = { 0, 180 };
	float srange[] = { 0, 256 };
	const float *ranges[] = { hrange, srange };

	cvtColor(a, a_hsv, CV_BGR2HSV);
	cvtColor(b, b_hsv, CV_BGR2HSV);

	calcHist(&a, 1, channels, Mat(), hista, 2, histsize, ranges, true, false);
	calcHist(&b, 1, channels, Mat(), histb, 2, histsize, ranges, true, false);
	return norm(hista, histb);
}

double SurfDistanceCalculator::calculate(Mat a, Mat b)
{
	Mat aGrayscale;
	Mat bGrayscale;

	// Convert to grayscale
	cvtColor(a, aGrayscale, CV_BGR2GRAY);
	cvtColor(b, bGrayscale, CV_BGR2GRAY);

	IplImage aIpl = aGrayscale;
	IplImage bIpl = bGrayscale;

	return surf(&aIpl, &bIpl);
}

int main(int argc, char **argv)
{
	string target_name = argv[1];
	string file_list_name = argc > 2 ? argv[2] : "lista.txt";

	// Read input file with the image database
	ifstream file_list;
	file_list.open(file_list_name.c_str(), ios::in);
	vector < string > imgs;
	string img_file_name;
	while(file_list >> img_file_name) {
		imgs.push_back(img_file_name);
	}

	// Calculate the distance between the input image to the database
	//HistogramDistanceCalculator d;
	SurfDistanceCalculator d;
	vector < result_data > distances = get_distances(target_name, imgs, &d);
	for(size_t i = 0; i < distances.size(); i++) {
		cout << i << " - " << distances[i].first << " -- " << distances[i].second << std::endl;
	}

	// Show the best 10 pairs
	for(size_t i = 0; i < 10; i++) {
		string windowName = distances[i].second.c_str();
		namedWindow(windowName, CV_WINDOW_AUTOSIZE);
		cvMoveWindow(windowName.c_str(), (i%5)*260, (i/5)*280);
		imshow(windowName, imread(windowName));
	}

	// Show input img
	namedWindow(target_name, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(target_name.c_str(), 0, 2*280);
	imshow(target_name, imread(target_name));

	cvWaitKey();
	return 0;
}

/*
 * Returns a sorted result_data vector with de distances of the image in
 * target_name image file to the image files in imgs vector.
 * */
vector < result_data > get_distances(string target_name, vector < string > imgs,
		DistanceCalculator * d)	{
	vector < result_data > distances;
	const Mat target_img = imread(target_name);

	for(size_t i = 0; i < imgs.size(); i++) {
		printf("Calculating distance for: %s\n", imgs[i].c_str());
		const Mat img = imread(imgs[i]);
		double dist = d->calculate(target_img, img);
		distances.push_back(result_data(dist, imgs[i]));
	}
	sort(distances.begin(), distances.end(), sort_results);
	return distances;
}

/*
 * Sort results in lexical order of distances.
 */
bool sort_results(const result_data & left, const result_data & right)
{
	return left.first < right.first;
}
