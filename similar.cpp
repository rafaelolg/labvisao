#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
 
#include "similar.h"

using namespace std;
using namespace cv;

class HistogramDistanceCalculator : public DistanceCalculator
{
    public:
        virtual double calculate(Mat a,Mat b){
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


};


class SurfDistanceCalculator : public DistanceCalculator
{
    public:
        virtual double calculate(Mat a,Mat b){
            return 0.0;
        }
};


int main( int argc, char** argv )
{
    string target_name = argv[1];
    string file_list_name = argc >2? argv[2] : "lista.txt";

    ifstream file_list;
    file_list.open(file_list_name.c_str(), ios::in);
    vector<string>  imgs;
    string img_file_name;
    while (file_list >> img_file_name) {
        imgs.push_back(img_file_name);
    }
    HistogramDistanceCalculator d;
    vector<result_data> distances = get_distances(target_name, imgs, &d);
    for(size_t i = 0; i < distances.size(); i++) {
        cout << i << " - " << distances[i].first << " -- " << distances[i].second << std::endl;
    }
    return 0;
}




vector<result_data> get_distances(string target_name, vector<string> imgs, DistanceCalculator * d) {
        vector<result_data> distances;
        const Mat target_img = imread(target_name);

        for(size_t i = 0; i < imgs.size(); i++) {
            const Mat img = imread(imgs[i]);
            double dist = d->calculate(target_img, img);
            distances.push_back(result_data(dist, imgs[i]));
        }
        sort(distances.begin(), distances.end(), sort_results);
        return distances;
}

bool sort_results(const result_data& left, const result_data& right) {
    return left.first < right.first;
}

