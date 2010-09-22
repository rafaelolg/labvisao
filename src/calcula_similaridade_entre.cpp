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
    string img1_name = argv[1];
    string img2_name = argv[2];
    int m= atoi(argv[3]);
    double s;
    
    Mat img1 = imread(img1_name);
    Mat img2 = imread(img2_name);

    HistogramSimilarityCalculator a;
    LocalizedSimilarityCalculator b;
    LocalizedMomentsSimilarityCalculator c;

    SimilarityCalculator * metodos[4] = {&a, &b, &c};

    s = metodos[m]->calculate(img1, img2);

    printf("%lf\n", s);
    cerr<< ".";



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
