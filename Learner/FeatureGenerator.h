// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#ifndef __FEATUREGENERATOR_H__
#define __FEATUREGENERATOR_H__

#include <cv.h>
#include <opencv2/opencv.hpp>
#include "ImageProcessing.h"


namespace CRVL
{
	void make_HOG(cv::Mat _src, cv::Mat& _outputFeature);
    void make_HOG_64(cv::Mat _src, cv::Mat& _outputFeature);
    void make_HOG_256(cv::Mat _src, cv::Mat& _outputFeature);

	void make_LightChnFtrs(cv::Mat _src, cv::Mat& _outputFeature);
	void make_LightChnFtrs_HOG(cv::Mat _src, cv::Mat& _outputFeature);
    void make_NoFeature(cv::Mat _src, cv::Mat& _outputFeature);
    void make_Sobel(cv::Mat _src, cv::Mat& _outputFeature);
}
#endif
