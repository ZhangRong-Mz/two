#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
IplImage * SrcImage = NULL;
IplImage * pSrcImage = NULL;
IplImage * img2 = NULL;
int su;
int Record1[999999] = { 0 },Record2[999999] = {0};
int Info[999999] = {};
const CvRect rect(200,120, 1700, 910);
void on_trackbar(int pos, IplImage *g_pGrayImage, IplImage *g_pBinaryImage)
{
	// תΪ��ֵͼ  
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	//תΪ�Ҷ�ͼ��
	//cvCvtColor(g_pGrayImage, g_pBinaryImage, CV_BGR2GRAY);
}
//ͳ������
int bSums(IplImage *src)
{
	int counter = 0;
	//�������������ص�  
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
//ͼƬ�������
void onTrackerSlid()
{
	
	char image_name[20];
	std::string path = "C:\\Users\\Mz\\Desktop\\����\\";
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
	sprintf(image_name, "%d��result%s", nn, ".jpg");//�����ͼƬ�� 
	std::string imageAddress = path + image_name;
	cvSaveImage(imageAddress.c_str(), img2);   //����һ֡ͼƬ
	Record1[nn]=Record2[nn] = bSums(img2);
}
void quick_sort(int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //���м��������͵�һ�������� �μ�ע1  
		int i = l;
		int j = r;
		int x = Record1[l];
		int flag = Info[l];
		while (i < j)
		{
			while (i < j && Record1[j] >= x) // ���������ҵ�һ��С��x����
			{
				j--;
			}
			if (i < j)
			{ 
				Record1[i] = Record1[j];
			    Info[i] = Info[j];
				i++;
			}

			while (i < j && Record1[i] < x) // ���������ҵ�һ�����ڵ���x����  
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
		quick_sort(l, i - 1); // �ݹ����   
		quick_sort(i + 1, r);
	}
}

void main()
{
	char image_name[20];
	int n=0;
	int m=0;
	const char *pstrWindowsSrcTitle = "ԭͼ";
	const char *pstrWindowsToolBarName = "��ֵͼ��ֵ";
	std::string path = "C:\\Users\\Mz\\Desktop\\����\\";
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
			if ((m%5)== 0)
			{
				n++;
				cap >> aft; // get a new frame from camera  
				if (aft.empty())
				{
					break;
				}
				
//Ԥ����				
				//��ֵ�˲�
				blur(pre, pre, Size(3, 3));
				blur(aft, aft, Size(3, 3));
				//
				IplImage *ssSrcImage = &IplImage(pre);
				IplImage *pppSrcImage = &IplImage(aft);
				//cvPyrMeanShiftFiltering(ssSrcImage, ssSrcImage, 25, 10, 2);
				IplImage *sSrcImage=cvCreateImage(cvGetSize(ssSrcImage),8, 1);
				cvCvtColor(ssSrcImage, sSrcImage, CV_BGR2GRAY);
				//cvNamedWindow("�Ҷ�ͼ��1", 2);
				//cvShowImage("�Ҷ�ͼ��1", sSrcImage);
				sprintf(image_name, "%d�ĻҶ�1%s", n, ".jpg");//�����ͼƬ�� 
				std::string imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), sSrcImage);   //����һ֡ͼƬ
				on_trackbar(80, sSrcImage, sSrcImage);
				//cvNamedWindow("��ֵ��1", 2);
				//cvShowImage("��ֵ��1", sSrcImage);
				sprintf(image_name, "%d�Ķ�ֵ��1%s", n, ".jpg");//�����ͼƬ�� 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(),sSrcImage);   //����һ֡ͼƬ
				SrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(sSrcImage, rect);//ѡȡ����Ȥ����
				cvCopy(sSrcImage, SrcImage);
				//int thresh = 80;
				//cvPyrMeanShiftFiltering(pppSrcImage, pppSrcImage, 25, 10, 2);
				IplImage *ppSrcImage= cvCreateImage(cvGetSize(pppSrcImage), IPL_DEPTH_8U, 1);
				cvCvtColor(pppSrcImage, ppSrcImage, CV_BGR2GRAY);
				//cvNamedWindow("�Ҷ�ͼ��2", 2);
				//cvShowImage("�Ҷ�ͼ��2", ppSrcImage);
				sprintf(image_name, "%d�ĻҶ�2%s", n, ".jpg");//�����ͼƬ�� 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), ppSrcImage);   //����һ֡ͼƬ
				on_trackbar(80, ppSrcImage, ppSrcImage);
				//cvNamedWindow("��ֵ��2", 2);
				//cvShowImage("��ֵ��2", ppSrcImage);
				sprintf(image_name, "%d�Ķ�ֵ��2%s", n, ".jpg");//�����ͼƬ�� 
				imageAddress = path + image_name;
				cvSaveImage(imageAddress.c_str(), ppSrcImage);   //����һ֡ͼƬ
//�����ȡ
				pSrcImage = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
				cvSetImageROI(ppSrcImage, rect);//ѡȡ����Ȥ����
				cvCopy(ppSrcImage, pSrcImage);
				img2 = cvCreateImage(cvGetSize(SrcImage), 8, 1);
				// int channel = img->nChannels;  
				// printf("the image is %d X %d wiht %d channels",height,width,channel);  
				//cvNamedWindow("SrcImage", 2);//��������  
				//cvNamedWindow("pSrcImage", 2);
				cvNamedWindow("result", 2);
				//cvShowImage("SrcImage", SrcImage);//��ʾͼ��  
				//cvShowImage("pSrcImage", pSrcImage);
				//cvCreateTrackbar("threshold", "result", &thresh, 255, onTrackerSlid);
				onTrackerSlid();
				cvWaitKey(0); //�ȴ�����  
    		    //cvDestroyWindow("SrcImage");//���ٴ���  
                //cvDestroyWindow("pSrcImage");
				//���¸�ֵ
				pre = aft.clone();
				//������ȡ
				vector<vector<Point>> contours;
				Mat matimg;
				matimg = cvarrToMat(sSrcImage);
				//find
				findContours(matimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				CvSeq *first_contour = NULL;
				CvMemStorage *storage = cvCreateMemStorage(0); //��ȡ������Ҫ�Ĵ�������ΪĬ��KB
				CvSeq * pcontour = 0; //��ȡ����������ָ��
				cvFindContours(sSrcImage, storage, &first_contour);
				while (contour = cvFindNextContour(contours))
				{
					tmparea = fabs(cvContourArea(contour));
					if (tmparea < minarea)
					{
						cvSubstituteContour(scanner, NULL);//ɾ����ǰ������  
					}
				}
				//draw
				Mat result(matimg.size(), CV_8U, Scalar(255));
				drawContours(result, contours, -1, Scalar(0), 2);
				CvSeq* contour = NULL;
				contourArea(result, false);
				sprintf(image_name, "%d contours %s", n, ".jpg");//�����ͼƬ�� 
				imageAddress = path + image_name;
				imwrite(imageAddress.c_str(), result);   //����һ֡ͼƬ
			    cvNamedWindow("contours",2);
				imshow("contours", result);
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
			printf("(%d)%d��%d\n", Info[i],Record1[i], su);
		}

}