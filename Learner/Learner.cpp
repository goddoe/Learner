// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#include "Learner.h"
//#include <Windows.h>

using namespace cv;
using namespace std;
using namespace sj;

namespace CRVL
{

    Learner::~Learner()
    {
        switch(m_learner){
        case LEARNER_SVM:
            break;
        case LEARNER_MLP:
            delete net;
            break;
        }
    }

    Learner::Learner()
    {

    }


    Learner::Learner(void (*getFeature)(Mat src, Mat& feature_CV_32FC1), int _learner)
	{
        this->getFeature    = getFeature;
        this->m_learner     = _learner;
    }

    void Learner::addData(char* _dir, int _label)
	{
        char* dir = new char[1024];
		strcpy(dir, _dir);

		m_folderList.push_back(dir);

		vector<char*> fileList;
        char filter[64] = IMAGE_EXTENSION;
        DirControl::makeFileList(dir, fileList,filter);
        //DirControl::makeFileList(dir, fileList);

		//////////////////////////////////////////////////////////////////////////
		// dir�� �α����� �Ǿ��־ �޸��� ������ ��� ����?
        // will released in train()
		for(int i = 0 ; i < fileList.size() ; ++i)
		{
            char* dirImg	= new char[1024];
			int label = _label;
			sprintf(dirImg, "%s/%s",dir, fileList.at(i));

			m_dirImg.push_back(dirImg);
			m_label.push_back(label);
		}
	}

    void Learner::train()
	{
        // check classes
        checkClasses();

        // make shuffle_check_table & initialize
        for(int i = 0 ; i < m_dirImg.size() ; ++i)
            m_shuffleCheckTable.push_back(false);

        m_shuffleCheckTable_count       = 0;
        m_dataNum                       = m_dirImg.size();

        // get train data's property
        Mat feature;
		Mat src = imread(m_dirImg.at(0));

        getFeature(src, feature);
		//////////////////////////////////////////////////////////////////////////
		//
		m_totalData = m_dirImg.size();

		int trainDataRows		= m_label.size();
        m_trainDimension		= feature.rows;
        m_trainData             = Mat::zeros(trainDataRows, m_trainDimension,CV_32FC1);
        m_trainLabel			= Mat::zeros(trainDataRows, 1,CV_32FC1);

        float* m_trainData_data = (float*)m_trainData.data;
        float* m_trainLabel_data = (float*)m_trainLabel.data;


        printf("m_trainDimension : %d \n", m_trainDimension);
		//////////////////////////////////////////////////////////////////////////
		// Make m_trainData, m_trainLabel
		for(int i = 0 ; i < m_totalData ; ++i)
        {
            m_trainLabel.at<float>(i)  = (float)m_label.at(i);

			//////////////////////////////////////////////////////////////////////////
			// Make Features

			//////////////////////////////////////////////////////////////////////////
			// Read Image

            Mat src = imread(m_dirImg.at(i));
			Mat feature;
			getFeature(src, feature);

            float* feature_data = (float*)feature.data;

			for(int j = 0 ; j < m_trainDimension; ++j)
            {
                m_trainData_data[i*m_trainDimension + j] = feature_data[j];

                //printf("m_m_trainData[%d][%d] : %f\n", i, j, m_trainData_data[i*m_trainDimension + j]);
            }
        }

        // learning start!
        printf("learn start...\n");
        learn();
        printf("learn done...!\n");

        printf("release start...\n");
        // release dirs
        for(int i = 0 ; i < m_dirImg.size(); ++i)
            delete m_dirImg.at(i);

        printf("release done...!\n");
	}


    float Learner::predict(cv::Mat _src)
	{
        //printf("%f\t",m_svm.predict(_src));
		return m_svm.predict(_src);
	}

    void Learner::save(char* _dir)
	{

        switch(m_learner)
        {
        case LEARNER_SVM:
            m_svm.save(_dir);
            break;
        case LEARNER_MLP:
            net->save(_dir);
            break;
        }
	}

    void Learner::load(char* _dir)
	{
		m_svm.load(_dir);
	}

    void Learner::learn()
    {
        switch(m_learner)
        {
        case LEARNER_SVM:
            learnSVM();
            break;
        case LEARNER_MLP:
            learnMLP();
            break;
        }


    }


    // Change this code!!! ======================
    void Learner::learnSVM()
    {
        //////////////////////////////////////////////////////////////////////////
        /*
        CvSVMParams params = CvSVMParams(
        CvSVM::C_SVC ,   // Type of SVM; using N classes here
        CvSVM::LINEAR,  // Kernel type
        0.0,            // Param (degree) for poly kernel only
        0.0,            // Param (gamma) for poly/rbf kernel only
        0.0,            // Param (coef0) for poly/sigmoid kernel only
        10,             // SVM optimization param C
        0,              // SVM optimization param nu (not used for N class SVM)
        0,              // SVM optimization param p (not used for N class SVM)
        NULL,           // class weights (or priors)
        */
        /*
        * Optional weights, assigned to particular classes.
        * They are multiplied by C and thus affect the misclassification
        * penalty for different classes. The larger the weight, the larger
        * the penalty on misclassification of data from the corresponding
        * class.
        */
        //cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001));


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // SVM trainning
        CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_EPS, 1000, 1e-6);

        // change here!!! =============================
        CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, NULL, NULL, NULL, 1.0, NULL, NULL, NULL, criteria);

        m_svm.train(m_trainData, m_trainLabel, cv::Mat(), cv::Mat(), param);
    }

    // Change this code!!! ======================
    void Learner::learnMLP()
    {
        // initialze Neural Net
        try
        {
            net = new Net(m_trainDimension);

            // Udaya you can modify these parameters
            //start

            net->m_eta = 0.012;             // Learning rate!
            //net->m_eta = 0.0005;
            net->m_alpha = 0.5;             // under 0.5
            net->m_weightRange = 0.2;       // under 0.5

            net->push_back_layer(800);      // add Layers!
            net->push_back_layer(400);
            net->push_back_layer(400);
            net->push_back_layer(100);

            // done
            net->push_back_layer(m_classes);

            net->setWeight();

            float *input  = new float[m_trainDimension];
            float *target = new float[m_classes];

            Mat tmpTrainData;
            float tmpTrainLabel;
            int tmp;

            for(int i = 0 ; i < m_trainData.rows ; ++i)
            {
                // change network parameter
                // SSudo dwego an Sudo dwego
                if(i%200 == 0){
                    net->m_eta *= 0.4;
                }

                tmp = shuffleData(tmpTrainData, tmpTrainLabel);

                if(tmp != tmpTrainLabel)
                    printf("ilegal\n");

                //printf("input:\n");
                for(int i = 0 ; i < m_trainDimension; ++i)
                {
                    input[i] = tmpTrainData.at<float>(i);
                }

                for(int i = 0 ; i < m_classes ; ++i)
                {
                    target[i] = -1.0;
                }

                target[(int)tmpTrainLabel] = 1.0;

                net->feedforward(input);
                net->backprob(target);
                printf("RecentAverageError: %f\n", net->getRecentAverageError());
            }

            delete input;
            delete target;
        }
        catch(exception e)
        {
            printf("%s\n",e.what());
            exit(0);
        }
    }

    void Learner::checkClasses()
    {
        vector<int> checkTable;
        m_maxClass =0;

        int count = 0;
        for(int i = 0 ; i < m_label.size(); ++i)
        {
            count = 0;
            for(int j = 0 ; j < checkTable.size() ; ++j )
            {
                if(checkTable[j] == m_label[i])
                {
                    ++count;

                    if( m_label[i] >= m_maxClass)
                        m_maxClass = m_label[i];

                }
            }
            if( count == 0 )
                checkTable.push_back(m_label[i]);
        }

        m_maxClass += 1;

        m_classes = checkTable.size();
        printf("m_classes: %d\n",m_classes);
        printf("m_maxClass: %d\n",m_maxClass);
    }


    // i should change this one because of train label
    float Learner::shuffleData(Mat& _trainData, float& _trainLabel )
    {
        srand((unsigned int) time(NULL));

        int randNum = 0 ;
        while(true)
        {
            randNum = rand() % m_dataNum;

            if(m_shuffleCheckTable[randNum] == false)
            {
                ++m_shuffleCheckTable_count;
                m_shuffleCheckTable[randNum] = true;
                break;
            }

            // if check table seted to true then break
            if(m_shuffleCheckTable_count >= m_dataNum)
                break;

        }
        _trainData = m_trainData(Rect(0,randNum,m_trainDimension,1));
        _trainLabel = m_trainLabel.at<float>(randNum);

        printf("_trainLabel: %d \n", (int)_trainLabel);
        return _trainLabel;

    }
}
