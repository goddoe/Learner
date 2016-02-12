// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "Classifier.h"

using namespace std;
using namespace cv;
using namespace CRVL;

int main(int argc, char *argv[])
{

   char learner_path[1024] = "data/MLP.txt";
   Classifier classifier(learner_path, LEARNER_MLP, make_HOG_64);

   Mat query = imread("data/neg.jpg");
   int response = classifier.recognize(query);

   printf("response : %d\n", response);

    return 0;
}
