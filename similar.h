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

class DistanceCalculator
{
    public:
        virtual ~DistanceCalculator() {}
        virtual double calculate(Mat a,Mat b) = 0;
};
class HistogramDistanceCalculator : public DistanceCalculator
{
    public:
        virtual double calculate(Mat a,Mat b);
};


class SurfDistanceCalculator : public DistanceCalculator
{
    public:
        virtual double calculate(Mat a,Mat b);
};

vector<result_data> get_distances(string target, vector<string> imgs, DistanceCalculator * d);
#endif
