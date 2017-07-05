#include <opencv2/opencv.hpp>
using namespace cv;

IplImage * SrcImage = NULL;
IplImage * pSrcImage = NULL;
IplImage * img2 = NULL;
int su;
const CvRect rect(200, 0, 1700, 1050);
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
void onTrackerSlid()
{
	
	char image_name[20];
	std::string path = "C:\\Users\\Mz\\Desktop\\测试静止帧的图片\\";
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
	printf("(%d)%d和%d\n",nn,bSums(img2),su);
}


void main()
{
	char image_name[20];
	int n=0;
	int m=0;
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsToolBarName = "二值图阈值";
	std::string path = "C:\\Users\\Mz\\Desktop\\测试静止帧的图片\\";
	VideoCapture cap("C:\\Users\\Mz\\Desktop\\Video\\3.avi");// open the default camera  
	if (!cap.isOpened()) // check if we succeeded  
		return;
	Mat pre;
	Mat aft;
	cap >> pre;
		for (;;)
		{
			m++;
			cap >> aft; // get a new frame from camera  
			if ((m%15)== 0)
			{
				n++;
				cap >> aft; // get a new frame from camera  
				if (aft.empty())
				{
					break;
				}
				IplImage *ssSrcImage = &IplImage(pre);
				cvPyrMeanShiftFiltering(ssSrcImage, ssSrcImage, 25, 10, 2);
				IplImage *sSrcImage=cvCreateImage(cvGetSize(ssSrcImage),8, 1);
				cvCvtColor(ssSrcImage, sSrcImage, CV_BGR2GRAY);
				//cvNamedWindow("灰度图像1", 2);
				//cvShowImage("灰度图像1", sSrcImage);
				sprintf(image_name, "%d的灰度1%s", n, ".jpg");//保存的图片名 
				std::string imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), sSrcImage);   //保存一帧图片
				on_trackbar(80, sSrcImage, sSrcImage);
				//cvNamedWindow("二值化1", 2);
				//cvShowImage("二值化1", sSrcImage);
				sprintf(image_name, "%d的二值化1%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(),sSrcImage);   //保存一帧图片
				SrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(sSrcImage, rect);//选取感兴趣区域
				cvCopy(sSrcImage, SrcImage);
				//处理
				//int thresh = 80;
				//载入图像  
				IplImage *pppSrcImage = &IplImage(aft);
				cvPyrMeanShiftFiltering(pppSrcImage, pppSrcImage, 25, 10, 2);
				IplImage *ppSrcImage= cvCreateImage(cvGetSize(pppSrcImage), IPL_DEPTH_8U, 1);
				cvCvtColor(pppSrcImage, ppSrcImage, CV_BGR2GRAY);
				//cvNamedWindow("灰度图像2", 2);
				//cvShowImage("灰度图像2", ppSrcImage);
				sprintf(image_name, "%d的灰度2%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), ppSrcImage);   //保存一帧图片
				on_trackbar(80, ppSrcImage, ppSrcImage);
				//cvNamedWindow("二值化2", 2);
				//cvShowImage("二值化2", ppSrcImage);
				sprintf(image_name, "%d的二值化2%s", n, ".jpg");//保存的图片名 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), ppSrcImage);   //保存一帧图片
				pSrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(ppSrcImage, rect);//选取感兴趣区域
				cvCopy(ppSrcImage, pSrcImage);
				img2 = cvCreateImage(cvGetSize(SrcImage), 8, 1);
				// int channel = img->nChannels;  
				// printf("the image is %d X %d wiht %d channels",height,width,channel);  
				//cvNamedWindow("SrcImage", 2);//创建窗口  
				//cvNamedWindow("pSrcImage", 2);
				cvNamedWindow("result", 2);
				//cvShowImage("SrcImage", SrcImage);//显示图像  
				//cvShowImage("pSrcImage", pSrcImage);
				//cvCreateTrackbar("threshold", "result", &thresh, 255, onTrackerSlid);
				onTrackerSlid();
				cvWaitKey(0); //等待按键  
    		    //cvDestroyWindow("SrcImage");//销毁窗口  
                //cvDestroyWindow("pSrcImage");
				//重新赋值
				pre = aft.clone();
			}
		}

}