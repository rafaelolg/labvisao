#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include "histogram_similarity.h"



double HistogramSimilarityCalculator::calculate(Mat a, Mat b)
{
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

