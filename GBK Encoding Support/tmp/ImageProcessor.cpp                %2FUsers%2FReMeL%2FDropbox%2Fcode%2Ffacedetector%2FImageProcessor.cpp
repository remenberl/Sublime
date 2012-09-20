#include "ImageProcessor.h"
#include "UIProxy.h"

ImageProcessor* ImageProcessor::processor = NULL;

ImageProcessor* ImageProcessor::getProcessor()
{
	if (!processor)
		processor = new ImageProcessor();
	return processor;
}

ImageProcessor::ImageProcessor(void)
{
	//@TEST for FaceDetector
	//cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("face",CV_WINDOW_AUTOSIZE);

	//@TEST for FaceRecognizer
	//cvNamedWindow("query face", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("target face",CV_WINDOW_AUTOSIZE);
}

ImageProcessor::~ImageProcessor(void)
{
	//@TEST for FaceDetector
	//cvDestroyWindow("src");
	//cvDestroyWindow("face");

	//@TEST for FaceRecognizer
	//cvDestroyWindow("query face");
	//cvDestroyWindow("target face");

}

IplImage* ImageProcessor::rotateImage(IplImage *src, double angle)
{
	IplImage* dst = cvCloneImage(src);
	int delta = 1;
	int opt = 0;	// 1： 旋转加缩放 0:  仅仅旋转
	double factor;	
	float m[6];

	// Matrix m looks like:
	//
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ]       [ A21  A22   b2 ]
	// 
	CvMat M = cvMat( 2, 3, CV_32F, m );
	int w = src->width;
	int h = src->height;
	if(opt)			// 旋转加缩放
		factor = (cos(angle) + 1.0)*2;
	else			// 仅仅旋转
		factor = 1;
	m[0] = (float)(factor*cos(-angle*2));
	m[1] = (float)(factor*sin(-angle*2));
	m[3] = -m[1];
	m[4] = m[0];
	// 将旋转中心移至图像中间
	m[2] = w * 0.5f;  
	m[5] = h * 0.5f;  
	// dst(x,y) = A * src(x,y) + b
	cvZero(dst);
	cvGetQuadrangleSubPix( src, dst, &M);
	return dst;
}

IplImage* ImageProcessor::getGrayImage(string fileName)
{
	IplImage *src = cvLoadImage(fileName.c_str());
	IplImage *dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvCvtColor(src, dst, CV_RGB2GRAY);
	cvReleaseImage(&src);
	return dst;
}

void ImageProcessor::vectorizeImage(string fileName, int width, int height, uchar* data)
{
	//those images may have three channels
	//IplImage* src = cvLoadImage(fileName.c_str());  -->ERROR
	IplImage* src = getGrayImage(fileName.c_str());
	//resize
	IplImage* dst = cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
	cvResize(src, dst, CV_INTER_LINEAR);
	cvReleaseImage(&src);
	//vectorize
	int dataIndex = 0;
	for (int y = 0; y < dst->height; y++)
	{
		uchar* rowPtr = (uchar *)(dst->imageData + y * dst->widthStep);
		for (int x = 0; x < dst->width; x++)
			data[dataIndex++] = rowPtr[x];
	}
}

double ImageProcessor::getAngle(CvPoint p1, CvPoint p2)
{
	return atan(double (p2.y - p1.y) / double(p2.x - p1.x));
}

IplImage* ImageProcessor::reSize(IplImage *src, CvSize size)
{

	IplImage* dst = cvCreateImage(size, src->depth, src->nChannels);
	cvResize(src, dst, CV_INTER_LINEAR);
	cvReleaseImage(&src);
	return dst;
}

//@TEST for ObjectDetector
void ImageProcessor::showImage(IplImage *src)
{
	//cvNamedWindow("tmp",CV_WINDOW_AUTOSIZE);
	//cvShowImage("tmp",src);
	//cvWaitKey(0);
	//cvDestroyWindow("tmp");
}

//@TEST for FaceDetector
void ImageProcessor::showSrc(IplImage *src)
{
	//cvShowImage("src",src);
	//cvWaitKey(1);
}

void ImageProcessor::showSrc(string fileName)
{
    UIProxy proxy;
    proxy.setLeftImage(fileName.c_str());
}

//@TEST for FaceDetector
void ImageProcessor::showFace(IplImage *face)
{
	//cvDestroyWindow("face");
	//cvNamedWindow("face",CV_WINDOW_AUTOSIZE);
	//cvShowImage("face",face);
	//cvWaitKey(1);
}

void ImageProcessor::showFace(string fileName)
{
    UIProxy proxy;
	//crop category name and file name from "fileName"
	stringstream ss(fileName);
	string sub_str, catogory_name, file_name;
	while(getline(ss,sub_str,'/'))  //以|为间隔分割test的内容
	{
		catogory_name = file_name;
		file_name = sub_str;
	}
    proxy.setRightImage(string("data/Face/" + catogory_name + "/" + file_name).c_str());
}

//@TEST for FaceRecognizer
void ImageProcessor::showQuery(string fileName)
{
    /*
	IplImage *img = cvLoadImage(fileName.c_str());
	//check for resize
	CvSize size = cvGetSize(img);
	int max_size = 400;
	if (size.width > max_size || size.height > max_size)
	{
		if (size.width > size.height)
		{
			size.height = int(size.height * ((double)max_size / size.width));
			size.width = max_size;
		}
		else
		{
			size.width = int(size.width * ((double)max_size / size.height));
			size.height = max_size;
		}
		img = reSize(img, size);
	}
	cvShowImage("query face",img);
	cvWaitKey(1);
	cvReleaseImage(&img);
    */
    UIProxy proxy;
    proxy.setLeftImage(fileName.c_str());
}

//@TEST for FaceRecognizer
void ImageProcessor::showTarget(string fileName)
{
    /*
	IplImage *img = cvLoadImage(fileName.c_str());
	CvSize size = cvGetSize(img);
	int max_size = 400;
	if (size.width > max_size || size.height > max_size)
	{
		if (size.width > size.height)
		{
			size.height = int(size.height * ((double)max_size / size.width));
			size.width = max_size;
		}
		else
		{
			size.width = int(size.width * ((double)max_size / size.height));
			size.height = max_size;
		}
		img = reSize(img, size);
	}
	cvShowImage("target face",img);
	cvWaitKey(1);
	cvReleaseImage(&img);
    */
    UIProxy proxy;
    proxy.setRightImage(fileName.c_str());
}

void ImageProcessor::showLog(std::string log)
{
    UIProxy proxy;
    proxy.appendLog(log.c_str());
}