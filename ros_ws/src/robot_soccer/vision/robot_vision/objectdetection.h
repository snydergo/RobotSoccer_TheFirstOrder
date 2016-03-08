#pragma once

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

#include "utils.h"
#include "hsvcolorsubspace.h"

typedef std::vector<std::vector<cv::Point>> contour_vector_t;

void CreateControlWindow(const HsvColorSubSpace& colorSegment);

cv::VideoCapture ConnectCamera(std::string uri);

cv::Mat ReadFrame(cv::VideoCapture& camera);//frame in HSV

cv::Mat ColorSegmentImage(const cv::Mat &src, const HsvColorSubSpace& colorSegment);

contour_vector_t EdgeDetectImage(const cv::Mat& src);

std::vector<cv::Moments> GetMoments(contour_vector_t contours);

cv::Point2f GetMomentCenter(cv::Moments moments);

double GetMomentArea(cv::Moments moments);

