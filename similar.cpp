#include "cv.h"
#include "highgui.h"

using namespace cv;

int main( int argc, char** argv )
{
    const char* imagename = argc > 1 ? argv[1] : "lena.jpg";
    Mat img = imread(imagename);
    if( !img.data ) // check if the image has been loaded properly
        return -1;

    imshow("image", img);
    waitKey();
    return 0;
}
