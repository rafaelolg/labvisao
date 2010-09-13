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
};


vector < result_data > get_similarities(Mat target_img, vector <Mat> imgs, vector<string> img_names,  SimilarityCalculator * d);

#endif


