#ifndef LOCALIZEDSIMILARITYCALCULATOR_H_
#define LOCALIZEDSIMILARITYCALCULATOR_H_

#include "similar.h"

class LocalizedSimilarityCalculator: public SimilarityCalculator {
public:
	virtual double calculate(Mat a,Mat b);

	Mat rectangleForIndex(Mat mat,int index);
	Mat centralRectangle(Mat mat);

	double rectangleCalculate(Mat a, Mat b);
	double centralWindowCalculate(Mat a, Mat b);
	double slidingWindowCalculate(Mat a,Mat b);

	double distanceBetween(Mat a,Mat b);

	Mat cloneMat(Mat src, int x0, int y0, int x1, int y1);
};

#endif /* LOCALIZEDSIMILARITYCALCULATOR_H_ */
