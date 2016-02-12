#include "ImageProcessing.h"

using namespace std;

namespace CRVL
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// _src		:	intput , CV_8UC1;
	// _circle	:	output , Vec3f
	//
	void findCircle(Mat& _src, Vec3f& _circle)
	{
		bool flagTop	=	false;
		bool flagBottom	=	false;
		bool flagLeft	=	false;
		bool flagRight	=	false;

		Point2f ptTop;
		Point2f ptBottom;
		Point2f ptRight;
		Point2f ptLeft;

		Point2f ptCenter;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//initialize center
		ptCenter.x = _src.cols/2.0;
		ptCenter.y = _src.rows/2.0;


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//initial find center
		for(int i = 0 ; i < 100 ; i++)
		{
			for(int _y = 0 ; _y < _src.rows ; ++_y)
			{
				if(_src.at<uchar>(_y, (int)ptCenter.x) > 0)
				{
					flagTop = true;
				}

				if(flagTop == true && _src.at<uchar>(_y, (int)ptCenter.x) ==0 )
				{
					ptTop.y = _y;
					flagTop = false;
					break;
				}

				if(_y >_src.rows/2)
				{
					ptTop.y = 0;
					flagTop = false;
					break;
				}
			}

			for(int _y = _src.rows-1 ; _y > 0 ; --_y )
			{
				if(_src.at<uchar>(_y, (int)ptCenter.x) > 0)
				{
					flagBottom = true;
				}

				if(flagBottom == true && _src.at<uchar>(_y, (int)ptCenter.x) ==0 )
				{
					ptBottom.y = _y;
					flagBottom = false;
					break;
				}

				if(_y < _src.rows/2)
				{
					ptBottom.y = _src.rows-1;
					flagBottom = false;
					break;

				}
			}

			ptCenter.y = (ptTop.y + ptBottom.y)/2.0; 

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			for(int _x = 0 ; _x < _src.cols ; ++_x)
			{
				if(_src.at<uchar>((int)ptCenter.y, _x ) > 0)
				{
					flagLeft = true;
				}

				if(flagLeft == true && _src.at<uchar>((int)ptCenter.y, _x) ==0 )
				{
					ptLeft.x = _x;
					flagLeft = false;
					break;
				}

				if(_x > _src.cols/2)
				{
					ptLeft.x =  0;
					flagLeft = false;
					break;
				}
			}

			for(int _x = _src.cols-1 ; _x > 0 ; --_x )
			{
				if(_src.at<uchar>((int)ptCenter.y, _x) > 0)
				{
					flagRight = true;
				}

				if(flagRight == true && _src.at<uchar>((int)ptCenter.y, _x) ==0 )
				{
					ptRight.x = _x;
					flagRight = false;
					break;
				}

				if(_x < _src.cols/2)
				{
					ptRight.x = _src.cols-1;
					flagRight = false;
					break;
				}
			}
			ptCenter.x = (ptLeft.x + ptRight.x)/2.0;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		_circle[0] = ptCenter.x;
		_circle[1] = ptCenter.y;
		//short Radius
		(ptBottom.y - ptTop.y) > (ptRight.x - ptLeft.x) ? _circle[2] = (ptRight.x - ptLeft.x)/2.0 : _circle[2] = (ptBottom.y - ptTop.y)/2.0 ;
	}




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// src				:	input , CV_8UC3
	// insideResult		:	output 
	//
	void cropInsideCircle(cv::Mat& src , cv::Mat& insideResult)
	{
		resize(src,src, Size(200, 200), INTER_LANCZOS4);

		Mat srcClone = src.clone();
		Mat result(src.rows,src.cols, CV_8UC1);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// color range
		int bar_h_high=172;
		int bar_h_low=8;
		int bar_s_high=255;
		int bar_s_low=120;
		int bar_v_high=255;
		int bar_v_low=50;


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// erase fake red
		for(int _y = 0 ; _y < src.rows ; ++_y)
		{
			for(int _x = 0 ; _x < src.cols ; ++_x)
			{
				if(src.at<Vec3b>(_y, _x)[0]<255)
					src.at<Vec3b>(_y, _x)[0] += 1;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///BGR2HSV
		cvtColor(src, src, CV_BGR2HSV);


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// threshold
		for(int _y = 0 ; _y < src.rows ; ++_y)
		{
			for(int _x = 0; _x < src.cols ; ++_x)
			{
				if( src.at<Vec3b>(_y,_x)[1] >=bar_s_low && src.at<Vec3b>(_y,_x)[2] >=bar_v_low &&
					src.at<Vec3b>(_y,_x)[1] <=bar_s_high && src.at<Vec3b>(_y,_x)[2] <=bar_v_high &&
					(src.at<Vec3b>(_y,_x)[0] >=bar_h_high || src.at<Vec3b>(_y,_x)[0] <=bar_h_low))
					//result.at<uchar>(_y,_x) = 179-src.at<Vec3b>(_y,_x)[0];
					result.at<uchar>(_y,_x) =255;//- src.at<Vec3b>(_y,_x)[0];
				else
					result.at<uchar>(_y,_x) =0;
				//cout<<(179-src.at<Vec3b>(_y,_x)[0])<<endl;
			}

		}

		//////////////////////////////////////////////////////////////////////////
		//erode
		Mat element(3,3, CV_8U, cv::Scalar(1));
		erode(result,result,element);
		dilate(result,result,element);

		erode(result,result,element);
		dilate(result,result,element);

		erode(result,result,element);
		dilate(result,result,element);

		/////////////////////////////////////////////////////////////////////////
		//find Circle
		Vec3f centerCircle;
		CRVL::findCircle(result, centerCircle);

		//#define DRAWCIRCLE
#ifdef DRAWCIRCLE
		//////////////////////////////////////////////////////////////////////
		//draw circle
		Mat result_RGB;
		cvtColor(result, result_RGB, CV_GRAY2BGR);
		imshow("result",result);


		circle( result_RGB, Point(centerCircle[0], centerCircle[1]), centerCircle[2], Scalar(0,0,255), 1, CV_AA);
		circle( result_RGB, Point(centerCircle[0], centerCircle[1]), 2, Scalar(0,255,0), 1, CV_AA);

        imshow("detected circles", result_RGB);
#endif

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//crop inside region

		Mat insideImg(src.rows, src.cols, CV_8UC3);


		//bounds
		int bound_left = src.cols;
		int bound_right = 0;
		int bound_top = src.rows;
		int bound_bottom = 0;

		Rect insideRoi;

		Vec3f c = centerCircle;
		for(int _y = c[1] - c[2]; _y < c[1] + c[2]; ++_y )
		{
			long double a = c[2]*c[2]- (_y-c[1])*(_y-c[1]);
			//for float point calculate
			if(a <0)
				continue;

			long double value= std::sqrt(a);
			int _x_end = value + c[0];
			int _x_start = (-1) * value + c[0];

			for(int _x = _x_start; _x < _x_end; ++_x)
			{
				if(_y < src.rows && _y >= 0 && _x <src.cols && _x >= 0)
				{
					insideImg.at<Vec3b>(_y,_x)[0] = srcClone.at<Vec3b>(_y, _x)[0];
					insideImg.at<Vec3b>(_y,_x)[1] = srcClone.at<Vec3b>(_y, _x)[1];
					insideImg.at<Vec3b>(_y,_x)[2] = srcClone.at<Vec3b>(_y, _x)[2];
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//bound box left right
			if(_x_end > bound_right)
			{
				bound_right = _x_end;
			}
			if(_x_start < bound_left)
			{
				bound_left = _x_start;
			}

		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//bound box top bottom
		bound_top = c[1]-c[2];
		bound_bottom = c[1]+c[2] -1;

		insideRoi.x = bound_left;
		insideRoi.y = bound_top;
		insideRoi.width = bound_right - bound_left;
		insideRoi.height = bound_bottom - bound_top;

		//cout<<"boud_left : " << bound_left <<endl;
		//cout<<"bound_top : " << bound_top <<endl;
		//cout<<"width : " << insideRoi.width <<endl;
		//cout<<"height : " << insideRoi.height <<endl;

		insideResult = insideImg(insideRoi).clone();


		//imshow("croped src", insideImg);
		//imshow("insideResult", insideResult);

	}

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
				{
					1,0,-1
				},
				{
					2,0,-2
					},
					{
						1,0,-1
					}
			}
			,
			{
				{
					1,2,1
				}
				,
				{
					0,0,0
				}
				,
				{
					-1,-2,-1
				}
			}
			,
			{
				{
					2,1,1
				}
				,
				{
					1,0,-1
				}
				,
				{
					-1,-1,-2
				}
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
//                        if((srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] >172||srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] <13) &&
//                            srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+1] > 3 &&
//                            srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+2]> 1 )
//							srcChannel_H_data[_y*_src.cols + _x] =255;


                        if((srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] >172||srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+0] <13) &&
                            srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+1] > 80 &&
                            srcChannel_HSV_data[_y*(_src.cols*3)+ _x*3+2]> 30 )
                            srcChannel_H_data[_y*_src.cols + _x] =255;
					}
				}
//                Mat element(3,3, CV_8U, cv::Scalar(1));
// 
//                dilate(srcChannel_H,srcChannel_H,element);
//                erode(srcChannel_H,srcChannel_H,element);
// //  				
//  				dilate(srcChannel_H,srcChannel_H,element);
// 				erode(srcChannel_H,srcChannel_H,element);
			}
			break;
		}


		_dst = srcChannel_H;
	}


	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// templateMatch
	float templateMatch(cv::Mat _src1, cv::Mat _src2, cv::Size _size)
	{

		Mat src1;
		Mat src2;
		resize(_src1, src1, _size,INTER_LANCZOS4);
		resize(_src2, src2, _size,INTER_LANCZOS4);

		float distSum = 0 ;
		float tempPixelDist=0;
		for(int _y = 0 ; _y < _size.height ; ++_y)
		{
			for (int _x = 0 ; _x <_size.width ; ++_x)
			{
				for(int color_i=0; color_i<3 ; ++color_i)
				{
					tempPixelDist=0;

					tempPixelDist += (float)src1.at<Vec3b>(_y,_x)[color_i];
					tempPixelDist -= (float)src2.at<Vec3b>(_y,_x)[color_i];

					if(tempPixelDist<0)
						tempPixelDist = tempPixelDist * (-1);

					distSum += tempPixelDist;
				}
			}
		}

		return (1.0f - (distSum / (255.0*3.0*_size.height*_size.width)));
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

		_outputFeature = HogFeat;

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
