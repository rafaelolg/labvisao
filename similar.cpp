#include "cv.h"
#include "highgui.h"
#include "stdio.h"

using namespace std;
using namespace cv;

double histogram_distance(Mat a, Mat  b);





int main( int argc, char** argv )
{
    const char * imga_name = argv[1];
    const char * imgb_name = argv[2];

    Mat imga = imread(imga_name);
    Mat imgb = imread(imgb_name);

    if( !imga.data || ! imgb.data) 
        return -1;


    imshow("A", imga);
    imshow("B", imgb);

    waitKey();
    float d = histogram_distance(imga, imgb);
    printf("d = %f\n", d);
    



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


