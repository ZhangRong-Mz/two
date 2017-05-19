#include <opencv2/opencv.hpp>
using namespace cv;

IplImage * SrcImage = NULL;
IplImage * pSrcImage = NULL;
IplImage * img2 = NULL;

IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
const char *pstrWindowsBinaryTitle = "二值图";
const CvRect rect(200, 0, 1700, 1050);
void on_trackbar(int pos)
{
	// 转为二值图  
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	// 显示二值图  
	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
}



void onTrackerSlid(int pos)
{
	int i, j;
	CvScalar s0, s1, s2;
	for (i = 0; i<SrcImage->height; i++)
		for (j = 0; j<SrcImage->width; j++)

		{
			s0 = cvGet2D(SrcImage, i, j); // get the (i,j) pixel value  
			s1 = cvGet2D(pSrcImage, i, j);
			s2 = cvGet2D(img2, i, j);
			// for(k=0;k<channel;k++)  
			if (fabs(s1.val[0] - s0.val[0])>pos)
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
	cvShowImage("result", img2);
}


void main()
{
	int n=0;
	int m=0;
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsToolBarName = "二值图阈值";
	VideoCapture cap("C:\\Users\\Mz\\Desktop\\Video\\3.avi");// open the default camera  
	if (!cap.isOpened()) // check if we succeeded  
		return;
	Mat pre;
	Mat aft;
	cap >> pre;
	pSrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	cvSetImageROI(pSrcImage, rect);//选取感兴趣区域
	SrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	cvSetImageROI(SrcImage, rect);//选取感兴趣区域
	img2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	cvSetImageROI(img2, rect);//选取感兴趣区域
		for (;;)
		{
			m++;
			cap >> aft; // get a new frame from camera  
			if ((m%26)== 0)
			{
				cap >> aft; // get a new frame from camera  
				if (aft.empty())
				{
					break;
				}
				IplImage *sSrcImage = &IplImage(pre);
				cvCopy(sSrcImage, SrcImage);
				//处理
				int thresh = 40;
				//载入图像  
				/*cvRect rect=(10, 20, 100, 50);*/
				//cvPyrMeanShiftFiltering(pSrcImage, SrcImage, 25, 10, 2);
				IplImage *ppSrcImage = &IplImage(aft);
				cvCopy(ppSrcImage, pSrcImage);
				img2 = cvCreateImage(cvGetSize(SrcImage), 8, 1);
				// int channel = img->nChannels;  
				// printf("the image is %d X %d wiht %d channels",height,width,channel);  
				//cvNamedWindow("SrcImage", 2);//创建窗口  
				//cvNamedWindow("pSrcImage", 2);
				cvNamedWindow("result", 2);
				//cvShowImage("SrcImage", SrcImage);//显示图像  
				//cvShowImage("pSrcImage", pSrcImage);
				cvCreateTrackbar("threshold", "result", &thresh, 255, onTrackerSlid);
				onTrackerSlid(thresh);
				cvWaitKey(0); //等待按键  
							  //cvDestroyWindow("SrcImage");//销毁窗口  
							  //cvDestroyWindow("pSrcImage");

				//图像二值化处理




				//重新赋值
				pre = aft.clone();
			}
		}

}