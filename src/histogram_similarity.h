#ifndef HISTOGRAM_SIMILARITY_H
#define HISTOGRAM_SIMILARITY_H

#include "similar.h"


class HistogramSimilarityCalculator : public SimilarityCalculator
{
    public:
        virtual double calculate(Mat a,Mat b);
};


#endif
