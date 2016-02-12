// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#include "Classifier.h"

using namespace cv;

namespace CRVL
{
	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
    // Recognition

    Classifier::Classifier (char* recognitionPath, int m_learner, void (*getFeature)(Mat src, Mat& feature_CV_32FC1) )
    {
        switch(m_learner)
        {
        case LEARNER_SVM:
            mode = LEARNER_SVM;
            svmRecognizer.load(recognitionPath);
            featureGenerator = getFeature;

            break;
        case LEARNER_MLP:
            mode = LEARNER_MLP;
            featureGenerator = getFeature;

            net = new sj::Net();
            net->load(recognitionPath);

            break;
        }
    }

	int Classifier::recognize(cv::Mat _src)
    {
        switch(mode){
        case LEARNER_SVM:
            {
				Mat feature;
				featureGenerator(_src, feature);

                if (svmRecognizer.predict(feature) >= 1)
					return true;
				else
					return false;
            }
            break;

        case LEARNER_MLP:
            {
                Mat feature;
                featureGenerator(_src,feature);

                net->feedforward((float*)feature.data);

                float max = -1;
                int response = -1;
                for(int i = 0 ; i < net->m_layersInfo.back()->numNeuron; ++i){

                    if( max < (float)(net->m_layersActivation.back()[i])){

                        max = (float)(net->m_layersActivation.back()[i]);
                        response = i;
                    }
                }
                return response;
            }
            break;
        }

        return -1;
    }
}
