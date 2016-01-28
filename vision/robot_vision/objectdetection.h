
#include "opencv2/imgproc/imgproc.hpp"

#include "utils.h"

using namespace cv;

typedef vector<vector<Point>> countour_vector_t;

void CreateControlWindow(const HsvColorSubSpace& colorSegment);

VideoCapture ConnectCamera(char* uri = "");

Mat ReadFrame(VideoCapture camera);//frame in HSV

Mat ReadImage(char* = "");// image in HSV

Mat ColorSegmentImage(const Mat& src, const HsvColorSubSpace& colorSegment);

countour_vector_t EdgeDetectImage(const Mat& src);

vector<Moments> GetMoments(contour_vector_t contours);

Point2f GetMomentCenter(Moments moments);

vector<int> GetMomentArea(Moments moments);


