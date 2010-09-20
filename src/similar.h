#ifndef SIMILAR_H
#define SIMILAR_H
#include "cv.h"
#include "highgui.h"
#include <vector>
#include <utility>
#include <string>

using namespace std;
using namespace cv;


typedef pair<double, string> result_data;


bool sort_results(const result_data& left, const result_data& right);

class SimilarityCalculator
{
    public:
        virtual ~SimilarityCalculator() {}
        virtual double calculate(Mat a,Mat b) = 0;
        vector < string > getMostSimilars(Mat target_img, vector <Mat> imgs, vector<string> img_names, size_t n);
        vector < result_data > calculateSimilarities(Mat target_img, vector <Mat> imgs, vector<string> img_names);
        void printSimilarities(vector<Mat> target_imgs, vector<string> taget_names, vector <Mat> imgs, vector<string> img_names);
};



#endif
