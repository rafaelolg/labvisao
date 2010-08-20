#include "cv.h"
#include "highgui.h"
#include "stdio.h"

using namespace std;




using namespace cv;

float histogram_distance(Mat a, Mat  b);

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



float histogram_distance(Mat a, Mat  b) {
    MatND  hista, histb;
    int histsize[] = {256, 256, 256};
    int channels[] = {0, 1, 2};
    float range[] = {0, 256};
    const float * ranges[] = {range,range,range};

    calcHist(&a, 1, channels, Mat(), hista, 2, histsize, ranges, true, false);
    calcHist(&b, 3, channels, Mat(), histb, 2, histsize, ranges, true, false);
    // return compareHist(hista, hista, CV_COMP_BHATTACHARYYA);
    return norm(hista, histb);
}


