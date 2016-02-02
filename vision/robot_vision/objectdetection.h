#pragma once

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "utils.h"
#include "hsvcolorsubspace.h"

using namespace cv;

typedef vector<vector<Point>> contour_vector_t;

void CreateControlWindow(const HsvColorSubSpace& colorSegment);

VideoCapture ConnectCamera(std::string uri = "default");

Mat ReadFrame(VideoCapture& camera);//frame in HSV

Mat ColorSegmentImage(const Mat& src, const HsvColorSubSpace& colorSegment);

contour_vector_t EdgeDetectImage(const Mat& src);

vector<Moments> GetMoments(contour_vector_t contours);

Point2f GetMomentCenter(Moments moments);

double GetMomentArea(Moments moments);

