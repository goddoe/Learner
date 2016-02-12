#include <cv.hpp>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Net.h"
#include "ImageProcessing.h"
#include "DirControl.h"

#define TRAIN_CHNFTRS			(0)

#define LEARNER_SVM             (0)
#define LEARNER_MLP             (1)

#define IMAGE_EXTENSION         "*.jpg"

namespace CRVL
{
    class Learner
	{
    public:

        int                     m_learner;
        int                     m_classes;
        int                     m_maxClass;
        vector<int>             checkTable;


		std::vector<char*>		m_folderList;	// m_folderList.size() == number of classes
		std::vector<int >		m_label;
		std::vector<char* >		m_dirImg;

        Mat                     m_trainData;
        Mat                     m_trainLabel;

		int						m_totalData;
		int						m_trainDimension;

        std::vector<bool>       m_shuffleCheckTable;
        int                     m_shuffleCheckTable_count;
        int                     m_dataNum;
		//////////////////////////////////////////////////////////////////////////
		// ChnFtrs
		int m_channel_ChnFtrs ;
		cv::Size m_size_ChnFtrs;


        ~Learner();

        Learner();
        Learner( void (*getFeature)(Mat src, Mat& feature_CV_32FC1), int _learner);

        CvSVM m_svm;
        sj::Net* net;

        void (*getFeature)(Mat src, Mat& feature_CV_32FC1);
	
		void addData(char* _dir, int _label);
		void train();
        //void train(int method);
        void checkClasses();

        float shuffleData(Mat& _trainData, float& _trainLabel);

		float predict(Mat _src);
		void save(char* _dir);
		void load(char* _dir);

        void learn();

        void learnSVM();
        void learnMLP();
    };

}
