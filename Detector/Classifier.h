// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#ifndef __CLASSIFIER_H__
#define __CLASSIFIER_H__

#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include "Net.h"
#include "FeatureGenerator.h"

#define LEARNER_SVM             (0)
#define LEARNER_MLP             (1)

using namespace std;

namespace CRVL
{


	class Classifier
	{

	public:
		//////////////////////////////////////////////////////////////////////////
		//Variable Declare

        int mode;
        void (*featureGenerator)(Mat src, Mat& feature_CV_8UC1);

        sj::Net *net;
        CvSVM svmRecognizer;

		//////////////////////////////////////////////////////////////////////////
		// Constructor
		Classifier ()
		{

		}

        Classifier (char* recognitionPath, int m_learner, void (*getFeature)(Mat src, Mat& feature_CV_32FC1) );

		// Destructor
		~Classifier()
		{
            //만약 에러나면 이거 주석해
            if(net != NULL)
                delete net;
		}

		int recognize(cv::Mat _src);
	};



}
#endif //__CLASSIFIER_H__
