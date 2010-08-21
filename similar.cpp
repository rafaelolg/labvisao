#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
 

using namespace std;
using namespace cv;

double histogram_distance(Mat a, Mat  b);
typedef pair<double, string> result_data;


bool sort_results(const result_data& left, const result_data& right);


int main( int argc, char** argv )
{
    string target_name = argv[1];
    string file_list_name = argc >2? argv[2] : "lista.txt";

    ifstream file_list;

    file_list.open(file_list_name.c_str(), ios::in);
    vector<result_data> distances;
    
    Mat target_img = imread(target_name);


    string  img_file_name;
    while (file_list >> img_file_name) {
        Mat img = imread(img_file_name);
        double d = histogram_distance(target_img, img);
        distances.push_back(result_data(d, img_file_name));
    }

    sort(distances.begin(), distances.end(), sort_results);
    for(size_t i = 0; i < distances.size(); i++) {
        cout << i << " - " << distances[i].first << " -- " << distances[i].second << std::endl;
    }
    return 0;
}



double histogram_distance(Mat a, Mat  b) {
    MatND  hista, histb;
    Mat a_hsv, b_hsv;


    int histsize[] = {180, 256};
    int channels[] = {0, 1};
    float hrange[] = {0, 180};
    float srange[] = {0, 256};
    const float * ranges[] = {hrange,srange};

    cvtColor(a, a_hsv, CV_BGR2HSV);
    cvtColor(b, b_hsv, CV_BGR2HSV);

    calcHist(&a, 1, channels, Mat(), hista, 2, histsize, ranges, true, false);
    calcHist(&b, 3, channels, Mat(), histb, 2, histsize, ranges, true, false);
    return norm(hista, histb);
}


bool sort_results(const result_data& left, const result_data& right)
{
    return left.first < right.first;
}
