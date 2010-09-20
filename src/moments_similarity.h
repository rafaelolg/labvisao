#ifndef MOMENTSSIMILARITYCALCULATOR_H_
#define MOMENTSSIMILARITYCALCULATOR_H_

#include "similar.h"

class LocalizedMomentsSimilarityCalculator: public SimilarityCalculator {
public:
	virtual double calculate(Mat a,Mat b);

	Mat rectangleForIndex(Mat mat,int index);
	Mat centralRectangle(Mat mat);

	double rectangleCalculate(Mat a, Mat b);
	double centralWindowCalculate(Mat a, Mat b);
	double similarityBetween(Mat a,Mat b);

	Mat cloneMat(Mat src, int x0, int y0, int x1, int y1);
	Mat equalizedMat(Mat mat);
};

#endif /* MOMENTSSIMILARITYCALCULATOR_H_ */
