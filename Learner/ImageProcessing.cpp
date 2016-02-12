#include "ImageProcessing.h"

using namespace std;

namespace CRVL
{

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	void getGradientHistogram(cv::Mat _src, cv::Mat& _dst,int DIRECTION)
	{
		Mat src;
		cvtColor(_src, src, CV_BGR2GRAY);

		cv::Mat result(src.rows, src.cols, CV_8UC1);

		uchar* src_data		=	(uchar*)src.data;
		uchar* result_data	=	(uchar*)result.data; 

		int filter[GGH_DIRECTION_NUM][3][3]={
			{
                { 1, 0,-1 },
                { 2, 0,-2 },
                { 1, 0,-1 }
			}
			,
			{
                { 1, 2, 1 },
                { 0, 0, 0 },
                {-1,-2,-1}
			}
			,
			{
                { 2, 1, 1 },
                { 1, 0,-1 },
                {-1,-1,-2 }
			}
		};

		for(int _y = 1 ; _y < src.rows-1 ; ++_y)
		{
			for(int _x = 1 ; _x < src.cols-1 ; ++_x)
			{
				int tmp = 0;
				for(int __y =0 ; __y < 3 ; ++__y)
				{
					for(int __x = 0 ;__x < 3 ; ++__x)
					{
						//printf("src = %d\n",src_data[(_y-1 +__y)*(src.cols) + (_x-1 +__x)]);
						tmp	+= src_data[(_y-1 +__y)*(src.cols) + (_x-1 +__x)] * filter[DIRECTION][__y][__x];
					}
				}

				if(tmp>=255)
					result_data[_y*src.cols + _x] =255;
				else if(tmp <0)
					result_data[_y*src.cols + _x] = tmp*(-1);
				else
					result_data[_y*src.cols + _x] = tmp;

			}
		}

		_dst = result.clone();

	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	void getColorChannel(cv::Mat _src, cv::Mat& _dst, int CHANNEL)
	{
		Mat srcChannel_HSV;

		cvtColor(_src, srcChannel_HSV, CV_BGR2HSV);
		uchar* srcChannel_HSV_data = (uchar*)srcChannel_HSV.data;

		// only H space
		//Mat srcChannel_H(_src.rows, _src.cols, CV_8UC1);
		Mat srcChannel_H = Mat::zeros(_src.rows, _src.cols, CV_8UC1 );
		uchar* srcChannel_H_data = (uchar*)srcChannel_H.data;

		switch(CHANNEL)
		{
		case GGC_H:
			{
				for(int _y = 0 ; _y < _src.rows ; ++_y)
				{
					for(int _x = 0 ; _x < _src.cols ; ++_x)
					{
						if(srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0]>172 ||srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] <8)
							srcChannel_H_data[_y*_src.cols + _x] =255;
					}
				}

			}
			break;

		case GGC_S:
			{
				for(int _y = 0 ; _y < _src.rows ; ++_y)
				{
					for(int _x = 0 ; _x < _src.cols ; ++_x)
					{

						srcChannel_H_data[_y*_src.cols + _x] = srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+1];
					}
				}

			}
			break;
		case GGC_V:
			{
				for(int _y = 0 ; _y < _src.rows ; ++_y)
				{
					for(int _x = 0 ; _x < _src.cols ; ++_x)
					{

						srcChannel_H_data[_y*_src.cols + _x] = srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+2];
					}
				}
			}
			break;
		case GGC_RED:
			{
				for(int _y = 0 ; _y < _src.rows ; ++_y)
				{
					for(int _x = 0 ; _x < _src.cols ; ++_x)
					{
						if((srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] >172 ||srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] <8) &&
							srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+1] > 120 &&
							srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+2]>50 )
							srcChannel_H_data[_y*_src.cols + _x] =255;
					}
				}

			}
			break;
		}
		_dst = srcChannel_H;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	cv::Mat normalize1Channel(cv::Mat _src, cv::Mat& _dst, int nomalValue)
	{
		Mat src = _src.clone();
		Mat result = Mat::zeros(_src.rows, _src.cols, _src.type());
		//////////////////////////////////////////////////////////////////////////
		// normalize srcChannel_V
		float distSum = 0 ;
		float average = 0;
		for(int _y = 0 ; _y <  src.rows ; ++_y)
		{
			for (int _x = 0 ; _x <src.cols ; ++_x)
			{
				distSum += (float)src.at<uchar>(_y,_x);	
			}
		}

		average = distSum / (float)(src.rows * src.cols);

		for(int _y = 0 ; _y <  src.rows ; ++_y)
		{
			for (int _x = 0 ; _x <src.cols ; ++_x)
			{

				result.at<uchar>(_y,_x)  = src.at<uchar>(_y,_x) * ((float)nomalValue/average);

			}
		}

		_dst = result;
		return result;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// getHOG
	// 128, 256 과 같은 숫자들 추천
	Mat getHOG(Mat _src, Size _winSize)
	{
		resize(_src, _src, _winSize, INTER_LANCZOS4);

		vector<float> ders;
		vector<Point> locs;
		cv::HOGDescriptor hog(_winSize, Size(16,16), Size(8,8), Size(8,8), 9);
		hog.compute(_src, ders, Size(8,8), Size(0,0), locs);

		Mat HogFeat(ders.size(), 1, CV_32FC1);
		for(int i = 0 ; i < ders.size() ; i ++)
		{
			HogFeat.at<float>(i,0)= ders.at(i);
		}

		return HogFeat;
	}

	Mat getHOG(Mat _src, Size _winSize ,Mat& _outputFeature)
	{
		resize(_src, _src, _winSize, INTER_LANCZOS4);

		vector<float> ders;
		vector<Point> locs;
		cv::HOGDescriptor hog(_winSize, Size(16,16), Size(8,8), Size(8,8), 9);
		hog.compute(_src, ders, Size(8,8), Size(0,0), locs);

		Mat HogFeat(ders.size(), 1, CV_32FC1);
		for(int i = 0 ; i < ders.size() ; i ++)
		{
			HogFeat.at<float>(i,0)= ders.at(i);
		}

		_outputFeature = HogFeat.clone();

		return HogFeat;
	}


	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// Class

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// Labeling
	//////////////////////////////////////////////////////////////////////////
	// About Label


	//////////////////////////////////////////////////////////////////////////
	// About Labeling
	//////////////////////////////////////////////////////////////////////////
	//  나중에 성능좀 향상시키자
	void labeling(Mat& src, vector<Label*>& vecLabel)
	{
		// Only 1-Channel
		//if( src->nChannels != 1 )
		//      return;

		// image size load
		int height    = src.rows;
		int width    = src.cols;

		// input image, result image를 image size만큼 동적할당
		unsigned char*    inputImage    = new unsigned char    [height * width];
		int*            resultImage    = new int            [height * width];



		// before labeling prcess, initializing 
		for( int y = 0; y < height; y++ ){
			for( int x = 0; x < width; x++ ){
				// image copy
				inputImage[width * y + x] = src.at<uchar>(Point(x,y));

				// initialize result image
				resultImage[width * y + x] = 0;
			}
		}

#ifdef LABELING_PROCESS
		namedWindow("process", CV_WINDOW_NORMAL);
#endif
		//// 8-neighbor labeling
		// Labeling 과정에서 stack overflow 방지를 위한 stl <stack>사용 
		std::stack<cv::Point> st;
		int labelNumber = 0;
		int count = 0;
		for( int y = 1; y < height - 1; y++ ){
			for( int x = 1; x < width - 1; x++ ){
				// source image가 255일 경우 + Labeling 수행되지 않은 픽셀에서만 labeling process 시작
				if( inputImage[width * y + x] != 255 || resultImage[width * y + x] != 0 ) continue;

				labelNumber += 1;
				count++;

				int labelCount = 0;

				Label* label = new Label();
				(*label).label = labelNumber;

				vecLabel.push_back(label);

				int min_x = width;
				int max_x = 0;
				int min_y = height;
				int max_y = 0;

				//Mat targetImage = Mat::zeros(height, width, CV_8UC1);

				// 새로운 label seed를 stack에 push
				st.push(Point(x, y));

				// 해당 label seed가 labeling될 때(stack이 빌 때) 까지 수행
				while( !st.empty() ){
					// stack top의 label point를 받고 pop
					int ky = st.top().y;
					int kx = st.top().x;
					st.pop();

					// label seed의 label number를 result image에 저장
					resultImage[width * ky + kx] = 255;
					//targetImage.at<uchar>(Point(kx, ky)) = 255;

					// search 8-neighbor
					for( int ny = ky - 1; ny <= ky + 1; ny++ ){
						// y축 범위를 벗어나는 점 제외
						if( ny < 0 || ny >= height) continue;
						for( int nx = kx - 1; nx <= kx + 1; nx++ ){

							// x축 범위를 벗어나는 점 제외
							if( nx < 0|| nx >= width) continue;

							// source image가 값이 있고 labeling이 안된 좌표를 stack에 push
							if( inputImage[width * ny + nx] != 255 || resultImage[width * ny + nx] != 0 ) continue;
							st.push(Point(nx, ny));

							// 탐색한 픽셀이니 labeling
							resultImage[width * ny + nx] = 255;
							//targetImage.at<uchar>(Point(kx, ky)) =255;

							labelCount++;
#ifdef LABELING_PROCESS
							imshow("process", targetImage);
							waitKey(1);
#endif

							//오른쪽 왼쪽 위아래 바운드
							if(ny <= min_y){
								min_y = ny;
							}
							if(ny >= max_y)
							{
								max_y = ny;
							}

							if(nx <= min_x)
							{
								min_x = nx;
							}
							if(nx >= max_x)
							{
								max_x = nx;
							}
						}
					}
				}

				if( max_x - min_x <=1 || max_y - min_y <=1 )
					continue;

				(*label).rect.x = min_x;
				(*label).rect.y = min_y;
				(*label).rect.width = max_x - min_x;
				(*label).rect.height = max_y - min_y;
				(*label).count = labelCount;
			}
		}
		delete[] inputImage;
		delete[] resultImage;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
}
