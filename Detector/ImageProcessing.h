#ifndef __IMAGEPROCESSING_H__
#define __IMAGEPROCESSING_H__

#include <opencv2/opencv.hpp>
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

//#define SHOW_ALL_INFO

using namespace cv;

namespace CRVL
{
	
	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// only Function
	void findCircle(Mat& _src, Vec3f& _circle);
	void cropInsideCircle(cv::Mat& src , cv::Mat& insideResult);

	//////////////////////////////////////////////////////////////////////////
	// Get Gradient Histogram	
	void getGradientHistogram(cv::Mat _src, cv::Mat& _dst,int DIRECTION);
	void getColorChannel(cv::Mat _src, cv::Mat& _dst, int CHANNEL);

	cv::Mat normalize1Channel(cv::Mat _src, cv::Mat& _dst, int nomalValue);

	Mat getHOG(Mat _src, Size _winSize);

	Mat getHOG(Mat _src, Size _winSize, Mat& _outputFeature);

	//////////////////////////////////////////////////////////////////////////
	// return similarity <1.0
	float templateMatch(cv::Mat _src1, cv::Mat _src2, cv::Size _nomalizationSize);
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
