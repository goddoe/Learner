// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#ifndef __IMAGEPROCESSING_H__
#define __IMAGEPROCESSING_H__

#include <opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stack>

//#pragma  once

#define GGH_VERTICAL		0
#define GGH_HORIZONTAL		1
#define GGH_DIGONAL			2
#define GGH_DIRECTION_NUM	3

#define GGC_H				0
#define GGC_S				1
#define GGC_V				2
#define GGC_RED				3
using namespace cv;

namespace CRVL
{
	//////////////////////////////////////////////////////////////////////////
	// Get Gradient Histogram
	void getGradientHistogram(cv::Mat _src, cv::Mat& _dst,int DIRECTION);
	void getColorChannel(cv::Mat _src, cv::Mat& _dst, int CHANNEL);

	Mat getHOG(Mat _src, Size _winSize);
	Mat getHOG(Mat _src, Size _winSize, Mat& _outputFeature);

	cv::Mat normalize1Channel(cv::Mat _src, cv::Mat& _dst, int nomalValue);
	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// Class

	//////////////////////////////////////////////////////////////////////////
	// Labeling
	typedef struct Label
	{
		int count;
		int label;
		Rect rect;

	}Label;

	void labeling(Mat& binaryImg, std::vector<Label*>& vectorOfLabels);
}

#endif //__IMAGEPROCESSING_H__
