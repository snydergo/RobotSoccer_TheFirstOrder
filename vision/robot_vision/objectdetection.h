
#include "opencv2/imgproc/imgproc.hpp"

#include "utils.h"

void CreateControlWindow(Range hue, Range saturation, Range value);

VideoCapture ConnectCamera(char* uri = "");

Mat ReadFrame(VideoCapture camera);//frame in HSV

Mat ReadImage(char* = "");// image in HSV

Mat SegmentColor(Mat img, HsvColorSubSpace colorSegment);


