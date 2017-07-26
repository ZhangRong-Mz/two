#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
IplImage * SrcImage = NULL;
IplImage * pSrcImage = NULL;
IplImage * img2 = NULL;
IplImage * img = NULL;
int su;
int Record1[999999] = { 0 },Record2[999999] = {0};
int Info[999999] = {};
const CvRect rect(240,131, 1555, 852);
void on_trackbar(int pos, IplImage *g_pGrayImage, IplImage *g_pBinaryImage)
{
	// 转为二值图  
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	//转为灰度图像
	//cvCvtColor(g_pGrayImage, g_pBinaryImage, CV_BGR2GRAY);
}
//统计像素
int bSums(IplImage *src)
{
	int counter = 0;
	//迭代器访问像素点  
	su = 0;
	for (int i = 0; i < src->height; i++)
		for (int j = 0; j < src->width; j++,su++)
			if (cvGet2D(src, i, j).val[0] == 0)
			{
				counter++;
			}
	return counter;
}

int nn = 0;
//图片相减操作
void onTrackerSlid()
{
	
	char image_name[20];
	std::string path = "C:\\Users\\Mz\\Desktop\\测试2\\";
	int i, j;
	CvScalar s0, s1, s2;
	for (i = 0; i<SrcImage->height; i++)
		for (j = 0; j<SrcImage->width; j++)

		{
			s0 = cvGet2D(SrcImage, i, j); // get the (i,j) pixel value  
			s1 = cvGet2D(pSrcImage, i, j);
			s2 = cvGet2D(img2, i, j);
			// for(k=0;k<channel;k++)  
			if (fabs(s1.val[0] - s0.val[0])>0)
			{
				s2.val[0] = 0;
				cvSet2D(img2, i, j, s2); // set the (i,j) pixel value  
			}
			else
			{
				s2.val[0] = 255;
				cvSet2D(img2, i, j, s2);
			}
		}
	nn++;
	cvNamedWindow("result", 2);
	cvShowImage("result", img2);
	sprintf(image_name, "%d的result%s", nn, ".jpg");//保存的图片名 
	std::string imageAddress = path + image_name;
	cvSaveImage(imageAddress.c_str(), img2);   //保存一帧图片
	Record1[nn]=Record2[nn] = bSums(img2);
}
void quick_sort(int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1  
		int i = l;
		int j = r;
		int x = Record1[l];
		int flag = Info[l];
		while (i < j)
		{
			while (i < j && Record1[j] >= x) // 从右向左找第一个小于x的数
			{
				j--;
			}
			if (i < j)
			{ 
				Record1[i] = Record1[j];
			    Info[i] = Info[j];
				i++;
			}

			while (i < j && Record1[i] < x) // 从左向右找第一个大于等于x的数  
			{
				i++;
			}
			if (i < j)
			{
				Info[j] = Info[i];
				Record1[j] = Record1[i];
				j--;
			}
		}
		Record1[i] = x;
		Info[i] = flag;
		quick_sort(l, i - 1); // 递归调用   
		quick_sort(i + 1, r);
	}
}

void drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color)
{
	// 将检测到的直线在图上画出来 
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(image, pt1, pt2, color, 1); //  线条宽度设置
		printf("%d %d %d %d\n", pt1.x, pt1.y, pt2.x, pt2.y);
		++it;
	}
}
void main()
{
	Mat DstImg;
	char image_name[20];
	int n=0;
	int m=0;
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsToolBarName = "二值图阈值";
	std::string path = "C:\\Users\\Mz\\Desktop\\测试2\\";
	VideoCapture cap("C:\\Users\\Mz\\Desktop\\Video\\5.avi");// open the default camera  
	if (!cap.isOpened()) // check if we succeeded  
		return;
	Mat pre;
	Mat aft;
	cap >> pre;
		for (;;)
		{
			m++;
			cap >> aft; // get a new frame from camera  
			if ((m%5)== 0)
			{
				n++;
				cap >> aft; // get a new frame from camera  
				if (aft.empty())
				{
					break;
				}
				
//预处理				
				//均值滤波
				blur(pre, pre, Size(3, 3));
				blur(aft, aft, Size(3, 3));
				//
				IplImage *ssSrcImage = &IplImage(pre);
				IplImage *pppSrcImage = &IplImage(aft);
				//cvPyrMeanShiftFiltering(ssSrcImage, ssSrcImage, 25, 10, 2);
				IplImage *sSrcImage=cvCreateImage(cvGetSize(ssSrcImage),8, 1);
				cvCvtColor(ssSrcImage, sSrcImage, CV_BGR2GRAY);
				//cvNamedWindow("灰度图像1", 2);
				//cvShowImage("灰度图像1", sSrcImage);
				SrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(sSrcImage, rect);//选取感兴趣区域
				cvCopy(sSrcImage, SrcImage);

				sprintf(image_name, "%d的灰度1%s", n, ".jpg");//保存的图片名 
				std::string imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), SrcImage);   //保存一帧图片
				on_trackbar(80, SrcImage, SrcImage);
				Mat miSrcImage = cvarrToMat(SrcImage);
				DstImg = miSrcImage.clone();
				//cvNamedWindow("二值化1", 2);
				//cvShowImage("二值化1", sSrcImage);
				sprintf(image_name, "%d的二值化1%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(),SrcImage);   //保存一帧图片
				//int thresh = 80;
				//cvPyrMeanShiftFiltering(pppSrcImage, pppSrcImage, 25, 10, 2);
				IplImage *ppSrcImage= cvCreateImage(cvGetSize(pppSrcImage),8, 1);
				cvCvtColor(pppSrcImage, ppSrcImage, CV_BGR2GRAY);
				pSrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(ppSrcImage, rect);//选取感兴趣区域
				cvCopy(ppSrcImage, pSrcImage);
				//cvNamedWindow("灰度图像2", 2);
				//cvShowImage("灰度图像2", ppSrcImage);
				sprintf(image_name, "%d的灰度2%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), pSrcImage);   //保存一帧图片
				on_trackbar(80, pSrcImage, pSrcImage);
				//cvNamedWindow("二值化2", 2);
				//cvShowImage("二值化2", ppSrcImage);
				sprintf(image_name, "%d的二值化2%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), pSrcImage);   //保存一帧图片
//结果提取
				img2 = cvCreateImage(cvGetSize(SrcImage), 8, 1);
				// int channel = img->nChannels;  
				// printf("the image is %d X %d wiht %d channels",height,width,channel);  
				//cvNamedWindow("SrcImage", 2);//创建窗口  
				//cvNamedWindow("pSrcImage", 2);
				//cvShowImage("SrcImage", SrcImage);//显示图像  
				//cvShowImage("pSrcImage", pSrcImage);
				//cvCreateTrackbar("threshold", "result", &thresh, 255, onTrackerSlid);
				onTrackerSlid();
				cvWaitKey(0); //等待按键  
    		    //cvDestroyWindow("result");//销毁窗口  
				//重新赋值
				pre = aft.clone();
				//轮廓提取
				vector<vector<Point>> contours;
			//find
				Mat middle = cvarrToMat(SrcImage);
				Mat resultImage;
				Mat CannyImg;
				Canny(DstImg, CannyImg, 50, 200, 3);
			findContours(DstImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			vector<vector<Point> >::iterator itc = contours.begin();
			while (itc != contours.end())
			{
				if (itc->size()<500)                       //删除连通域面积小于500的轮廓连通域
				{
					itc = contours.erase(itc);
				}
				else
				{
					++itc;
				}
			}

				//draw
				Mat result(middle.size(), CV_8U, Scalar(255,255,255));
				drawContours(result, contours, -1, Scalar(0,0,0), 2);
				//cv::line(result, cv::Point(240,800), cv::Point(1795,800),Scalar(0));
				sprintf(image_name, "%d contours %s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				imwrite(imageAddress.c_str(), result);   //保存一帧图片

				Mat image;
				cvtColor(result,image, CV_GRAY2BGR);
				Mat I;
				cvtColor(image,I, CV_BGR2GRAY);
				Mat contours1;
				Canny(I, contours1, 125, 350);
				threshold(contours1, contours1, 128, 255, THRESH_BINARY);
				vector<Vec4i> lines;
				// 检测直线
				HoughLinesP(contours1, lines, 1, CV_PI / 180, 145, 300, 2000);
				printf("%d\n", lines.size());
				drawDetectLines(image,lines, Scalar(0, 255, 0));
				namedWindow("result", 2);
				imshow("result", image);
				sprintf(image_name, "%d HoughLinesP %s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				imwrite(imageAddress.c_str(),image);   //保存一帧图片


				/*
				vector<Vec4i> lines;
				HoughLinesP(result, lines, 1,CV_PI/360, 180, 30, 15);
				printf("%d", lines.size());
				Mat finalResult;
				cvtColor(result,finalResult, CV_GRAY2BGR);
				drawDetectLines(finalResult, lines, Scalar(0, 0,255));
				
				vector<Vec4i> Lines;
				HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
				for (size_t i = 0; i < Lines.size(); i++)
				{
					Vec4i l = Lines[i];
					line(DstImg, Point(l[0], l[1]), Point(l[2],l[3]), Scalar(23,180,55), 1, LINE_AA);
				}
				
			    cvNamedWindow("contours",2);
				imshow("contours", finalResult);
				sprintf(image_name, "finalResult.jpg");//保存的图片名 
				imageAddress = path + image_name;
				IplImage *res = &IplImage(finalResult);
				cvSaveImage(imageAddress.c_str(), res);   //保存一帧图片
				*/
				waitKey();
				
			}
		}

		for (int i = 1; i <= nn; i++)
		{
			Info[i] = i;
		}
		quick_sort(1,nn);
		for (int i = 1; i <= nn; i++)
		{
			printf("(%d)%d和%d\n", Info[i],Record1[i], su);
		}

}