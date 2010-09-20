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
#include "localized_similarity.h"
#include "moments_similarity.h"

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
        //cout << "loaded " << temp << std::endl;
    }
    while(search_list >> temp) {
        target_names.push_back(temp);
        target_imgs.push_back(imread(temp));
        //cout << "loaded " << temp << std::endl;
    }


    HistogramSimilarityCalculator d;
    //SurfSimilarityCalculator d;
    //LocalizedSimilarityCalculator d;
    //LocalizedMomentsSimilarityCalculator d;
    d.printSimilarities(target_imgs, target_names, imgs, img_names);
    //waitKey();
    return 0;
}




vector < string > SimilarityCalculator::getMostSimilars(Mat target_img, vector <Mat> imgs, vector<string> img_names, size_t n) {
        vector < result_data > similarities = this->calculateSimilarities(target_img, imgs, img_names);
        vector <string>   result;
        for(size_t i = 0; i < n; i++) {
            result.push_back(similarities[i].second);
        }
        return result;
}



/*
* Returns a sorted result_data vector with de similarities of the image in
* target_name image file to the image files in imgs vector.
* */
vector < result_data > SimilarityCalculator::calculateSimilarities(Mat target_img, vector <Mat> imgs, vector<string> img_names)
{
    vector < result_data > similarities;

    for(size_t i = 0; i < imgs.size(); i++) {
        double s = this->calculate(target_img, imgs[i]);
        similarities.push_back(result_data(s, img_names[i]));
    }
    sort(similarities.begin(), similarities.end(), sort_results);
    return similarities;
}


/*
 * Prints the most similar images file names to the stdout
 */

void  SimilarityCalculator::printSimilarities(vector<Mat> target_imgs, vector<string> target_names, vector <Mat> imgs, vector<string> img_names){
    for (size_t j = 0; j < target_imgs.size(); j++) {
        Mat target_img = target_imgs[j];
        string target_name = target_names[j];
        cout << target_name;
        vector < string > similars = this->getMostSimilars(target_img, imgs, img_names, 10);
        for(size_t i = 0; i < 10; i++) {
            cout << "\t" << similars[i];
        }
        cout << std::endl;
    }
}


/*
* Sort results in lexical order of similarities.
*/
bool sort_results(const result_data & left, const result_data & right)
{
    return left.first > right.first;
}
