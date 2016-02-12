#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "ImageProcessing.h"
#include "DirControl.h"
#include "Learner.h"
#include "FeatureGenerator.h"

#pragma once

using namespace cv;
using namespace std;
using namespace CRVL;
using namespace sj;

//Program helper
typedef void (*getFeaturePtr)(Mat src, Mat& feature_CV_32FC1);

int main(int argc, char* argv[]){

    // Leaner
    CRVL::Learner learner;

    learner.m_learner       = LEARNER_MLP;          // LEARNER_MLP, LEARNER_SVM
    learner.getFeature      = make_HOG_64;          // make_HOG_64, make_HOG256, ... in FeagureGenerator.h
    char save_path[1024]    = "svm.txt";// SAVE PATH

    // Training directory
    char train_dir[2][2048] = {"data/mlp_lg_with_padding/negative"
                              ,"data/mlp_lg_with_padding/positive"
                              };

    // Start!
    printf("start!\n");

    // Load Data
	printf("*******************************************************************************\n");
    printf("learner Load Data Start...!\n");

    // 클래스이름 1씩 증가!
    learner.addData(train_dir[0],0);
    learner.addData(train_dir[1],1);
    //learner.addData(train_dir[2],2);

    printf("learner Load Data Done...!\n");
    printf("*******************************************************************************\n\n");

    // Train
	printf("*******************************************************************************\n");
    printf("learner Train Start...!\n");
    learner.train();
    printf("learner Train Done...!\n");
    printf("*******************************************************************************\n\n");

    // Save
	printf("*******************************************************************************\n");
	printf("Save Start...!\n");

    learner.save(save_path);

	printf("Save Done...!\n");
	printf("*******************************************************************************\n\n");

	return 0;
}

