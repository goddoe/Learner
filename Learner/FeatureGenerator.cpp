#include "FeatureGenerator.h"

namespace CRVL
{
	void make_HOG(cv::Mat _src, cv::Mat& _outputFeature)
	{
        Mat src;

        cvtColor(_src,src, CV_BGR2GRAY);
        getHOG(src, Size(256,256), _outputFeature);
	}
    
    void make_HOG_64(cv::Mat _src, cv::Mat& _outputFeature)
	{
        Mat src;

        cvtColor(_src,src, CV_BGR2GRAY);
        getHOG(src, Size(64,64), _outputFeature);
	}
    
     void make_HOG_256(cv::Mat _src, cv::Mat& _outputFeature)
     {
         Mat src;
 
         cvtColor(_src,src, CV_BGR2GRAY);
         getHOG(src, Size(256,256), _outputFeature);
     }


    void make_NoFeature(cv::Mat _src, cv::Mat& _outputFeature)
    {
        resize(_src,_src, Size(50,50));
        _outputFeature = Mat(_src.rows*_src.cols*3 , 1 , CV_32FC1);
        for(int _y = 0 ; _y < _src.rows ; ++_y)
        {
            for(int _x = 0 ; _x < _src.cols; ++_x)
            {
                _outputFeature.at<float>(_y*_src.cols +_x)                        = (float)(_src.at<Vec3b>(_y ,_x)[0]/255.0);
                _outputFeature.at<float>(_y*_src.cols+_src.rows*_src.cols +_x)    = (float)(_src.at<Vec3b>(_y ,_x)[1]/255.0);
                _outputFeature.at<float>(_y*_src.cols+_src.rows*_src.cols*2 +_x)  = (float)(_src.at<Vec3b>(_y ,_x)[2]/255.0);
            }
        }
    }
    void make_Sobel(cv::Mat _src, cv::Mat& _outputFeature)
    {

        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Mat tmp;

        resize(_src, tmp, Size(20,20));

        _outputFeature = Mat(tmp.rows * tmp.cols, 1, CV_32FC1);

        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        GaussianBlur( tmp, tmp, Size(3,3), 0, 0, BORDER_DEFAULT );
        cvtColor(tmp,tmp, CV_BGR2GRAY);

        Sobel(tmp, grad_x, ddepth, 1,0,3, scale, delta, BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

        Sobel(tmp, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, tmp);

        float* _outputFeature_data  = (float*)_outputFeature.data;
        uchar* tmp_data             = (uchar*)tmp.data;


        //imshow("tmp",tmp);
        //waitKey(0);
        for(int i = 0 ; i < tmp.rows*tmp.cols; ++i)
        {
            _outputFeature_data[i] = (float)(tmp_data[i]/255.0);
        }


    }

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	void make_LightChnFtrs(cv::Mat _src, cv::Mat& _outputFeature)
	{
		//////////////////////////////////////////////////////////////////////////
		// Make Feature space
		Mat src = _src.clone();
		resize(src,src, Size(50,50), INTER_LANCZOS4);
		//imshow("_src",_src);
		Mat predictData(17500, 1 , CV_32FC1);

		//only H space 
		cv::Mat srcChannel_H;
		cv::Mat srcChannel_S;
		cv::Mat srcChannel_V;
		CRVL::getColorChannel(src, srcChannel_H, GGC_H);
		CRVL::getColorChannel(src, srcChannel_S, GGC_S);
		CRVL::getColorChannel(src, srcChannel_V, GGC_V);
		//////////////////////////////////////////////////////////////////////////
		// normalize srcChannel_V
		normalize1Channel(srcChannel_V, srcChannel_V, 125);

		// Edge space
		cv::Mat srcChannel_edge;
		Canny(src,srcChannel_edge,125,255,3);

		// Gradient Histogram
		cv::Mat srcChannel_vertical;
		cv::Mat srcChannel_horizontal;
		cv::Mat srcChannel_digonal;

		CRVL::getGradientHistogram(src, srcChannel_vertical, GGH_VERTICAL);
		CRVL::getGradientHistogram(src, srcChannel_horizontal, GGH_HORIZONTAL);
		CRVL::getGradientHistogram(src, srcChannel_digonal, GGH_DIGONAL);

		std::vector<cv::Mat> srcChannelArray;
		srcChannelArray.push_back(srcChannel_H);
		srcChannelArray.push_back(srcChannel_S);
		srcChannelArray.push_back(srcChannel_V);
		srcChannelArray.push_back(srcChannel_edge);
		srcChannelArray.push_back(srcChannel_vertical);
		srcChannelArray.push_back(srcChannel_horizontal);
		srcChannelArray.push_back(srcChannel_digonal);

		int count = 0;
		for(int j = 0 ; j < 7 ; ++j)
		{
			for(int y= 0; y < 50 ; ++y)
			{
				for(int x= 0; x < 50  ; ++x )
				{
					predictData.at<float>(count++,0) = (float)srcChannelArray.at(j).at<uchar>(y,x);

				}
			}
		}

		_outputFeature = predictData;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	void make_LightChnFtrs_HOG(cv::Mat _src, cv::Mat& _outputFeature)
	{
		//////////////////////////////////////////////////////////////////////////
		// Make Feature space
		Mat originSrc	= _src.clone();
		Mat src			= _src.clone();
		resize(src,src, Size(50,50), INTER_LANCZOS4);
		//imshow("_src",_src);
		Mat predictData(17500+34596, 1 , CV_32FC1);


		//only H space 
		cv::Mat srcChannel_H;
		cv::Mat srcChannel_S;
		cv::Mat srcChannel_V;
		CRVL::getColorChannel(src, srcChannel_H, GGC_H);
		CRVL::getColorChannel(src, srcChannel_S, GGC_S);
		CRVL::getColorChannel(src, srcChannel_V, GGC_V);

		normalize1Channel(srcChannel_V, srcChannel_V, 125);


		// Edge space
		cv::Mat srcChannel_edge;
		Canny(src,srcChannel_edge,125,255,3);

		// Gradient Histogram
		cv::Mat srcChannel_vertical;
		cv::Mat srcChannel_horizontal;
		cv::Mat srcChannel_digonal;

		CRVL::getGradientHistogram(src, srcChannel_vertical, GGH_VERTICAL);
		CRVL::getGradientHistogram(src, srcChannel_horizontal, GGH_HORIZONTAL);
		CRVL::getGradientHistogram(src, srcChannel_digonal, GGH_DIGONAL);

		std::vector<cv::Mat> srcChannelArray;
		srcChannelArray.push_back(srcChannel_H);
		srcChannelArray.push_back(srcChannel_S);
		srcChannelArray.push_back(srcChannel_V);
		srcChannelArray.push_back(srcChannel_edge);
		srcChannelArray.push_back(srcChannel_vertical);
		srcChannelArray.push_back(srcChannel_horizontal);
		srcChannelArray.push_back(srcChannel_digonal);

		int count = 0;
		for(int j = 0 ; j < 7 ; ++j)
		{
			for(int y= 0; y < 50 ; ++y)
			{
				for(int x= 0; x < 50  ; ++x )
				{
					predictData.at<float>(count++,0) = (float)srcChannelArray.at(j).at<uchar>(y,x);

				}
			}
		}
		Mat srcHogFeature;
		CRVL::getHOG(originSrc,Size(256,256), srcHogFeature);

		for(int i = 0 ; i < srcHogFeature.rows ; ++i)
		{
			predictData.at<float>(count++,0) = srcHogFeature.at<float>(i,0);
		}

		_outputFeature = predictData;


	}
}
