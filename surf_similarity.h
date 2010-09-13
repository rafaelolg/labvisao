#ifndef SURF_H_
#define SURF_H_
#include "similar.h"

class SurfSimilarityCalculator : public SimilarityCalculator
{
    public:
        virtual double calculate(Mat a,Mat b);
};



double surf(IplImage* object, IplImage* image);

#endif /* SURF_H_ */
