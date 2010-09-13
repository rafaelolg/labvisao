#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

#include "similar.h"
#include "surf_similarity.h"
#include "histogram_similarity.h"

using namespace std;
using namespace cv;



int main(int argc, char **argv)
{
    string search_list_name = argc > 1 ? argv[1] : "busca.txt";
    string file_list_name = argc > 2 ? argv[2] : "bd.txt";

    ifstream file_list;
    ifstream search_list;
    file_list.open(file_list_name.c_str(), ios::in);
    search_list.open(search_list_name.c_str(), ios::in);

    vector < string > img_names;
    vector < string > target_names;
    vector < Mat > imgs;
    vector < Mat > target_imgs;

    string temp;
    while(file_list >> temp) {
        img_names.push_back(temp);
        imgs.push_back(imread(temp));
        cout << "loaded" << temp << std::endl;
    }
    while(search_list >> temp) {
        target_names.push_back(temp);
        target_imgs.push_back(imread(temp));
        cout << "loaded" << temp << std::endl;
    }


    //HistogramSimilarityCalculator d;
    SurfSimilarityCalculator d;
    for (size_t j = 0; j < target_imgs.size(); j++) {
        Mat target_img = target_imgs[j];
        string target_name = target_names[j];
        vector < result_data > similarities = get_similarities(target_img, imgs, img_names, &d);
        cout << target_name;
        for(size_t i = 0; i < 10; i++) {
            cout << "\t" << similarities[i].second;
        }
        cout << std::endl;
    }
    cvWaitKey();
    return 0;
}

/*
 * Returns a sorted result_data vector with de similarities of the image in
 * target_name image file to the image files in imgs vector.
 * */
vector < result_data > get_similarities(Mat target_img, vector <Mat> imgs, vector<string> img_names,  SimilarityCalculator * d)
{
    vector < result_data > similarities;

    for(size_t i = 0; i < imgs.size(); i++) {
        double s = d->calculate(target_img, imgs[i]);
        similarities.push_back(result_data(s, img_names[i]));
    }
    sort(similarities.begin(), similarities.end(), sort_results);
    return similarities;
}

/*
 * Sort results in lexical order of similarities.
 */
bool sort_results(const result_data & left, const result_data & right)
{
    return left.first > right.first;
}
