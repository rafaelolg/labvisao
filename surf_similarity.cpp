#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include "surf_similarity.h"

using namespace std;

double SurfSimilarityCalculator::calculate(Mat a, Mat b)
{
	Mat aGrayscale;
	Mat bGrayscale;

	// Convert to grayscale
	cvtColor(a, aGrayscale, CV_BGR2GRAY);
	cvtColor(b, bGrayscale, CV_BGR2GRAY);

	IplImage aIpl = aGrayscale;
	IplImage bIpl = bGrayscale;

	double similarity = surf(&aIpl, &bIpl);
        return similarity;
}




double compareSURFDescriptors(
		const float* d1, const float* d2,
		double best, int length){
	double total_cost = 0;
	assert( length % 4 == 0 );
	for( int i = 0; i < length; i += 4 ) {
		double t0 = d1[i] - d2[i];
		double t1 = d1[i+1] - d2[i+1];
		double t2 = d1[i+2] - d2[i+2];
		double t3 = d1[i+3] - d2[i+3];
		total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
		if( total_cost > best )
			break;
	}
	return total_cost;
}


int naiveNearestNeighbor(const float* vec, int laplacian,
		const CvSeq* model_keypoints,
		const CvSeq* model_descriptors){

	int length = (int)(model_descriptors->elem_size/sizeof(float));
	int i, neighbor = -1;
	double d, dist1 = 1e6, dist2 = 1e6;
	CvSeqReader reader, kreader;
	cvStartReadSeq( model_keypoints, &kreader, 0 );
	cvStartReadSeq( model_descriptors, &reader, 0 );

	for(i = 0; i < model_descriptors->total; i++){
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* mvec = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		if( laplacian != kp->laplacian )
			continue;
		d = compareSURFDescriptors( vec, mvec, dist2, length );
		if( d < dist1 )
		{
			dist2 = dist1;
			dist1 = d;
			neighbor = i;
		}
		else if ( d < dist2 )
			dist2 = d;
	}
	if ( dist1 < 0.6*dist2 )
		return neighbor;
	return -1;
}

void findPairs(const CvSeq* objectKeypoints,
		const CvSeq* objectDescriptors, const CvSeq* imageKeypoints,
		const CvSeq* imageDescriptors,
		vector<int>& ptpairs){
	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq( objectKeypoints, &kreader );
	cvStartReadSeq( objectDescriptors, &reader );
	ptpairs.clear();

	for( i = 0; i < objectDescriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
		if( nearest_neighbor >= 0 )
		{
			ptpairs.push_back(i);
			ptpairs.push_back(nearest_neighbor);
		}
	}
}

double surf(IplImage* object, IplImage* image){
	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
	double similar = 0;

	CvSURFParams params = cvSURFParams(300, 1);
	//params.extended = 1;
	params.nOctaves = 3;
	params.nOctaveLayers = 3;
	//params.hessianThreshold = 300;

	cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
	cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

	vector<int> ptpairs;
	findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
	similar += ptpairs.size();

	// Resultado de 0 a 1
	return similar;
}
