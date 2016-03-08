#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/** @function main */
int main(int argc, char** argv)
{

  /// Read the image
  VideoCapture cap("http://192.168.1.78:8080/stream?topic=/image&dummy=param.mjpg"); //capture the video from web cam

    if (!cap.isOpened()) { // if not success, exit program 
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    float xMax = 0.0;
    float xMin = 10000.0;
    float yMax = 0.0;
    float yMin = 10000.0;
    Point2f averagedCenter;

    while (true) {

        Mat src, src_gray;
        bool bSuccess = cap.read(src); // read a new frame from video

        if (!bSuccess) { //if not success, break loop
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        /// Convert it to gray
        cvtColor( src, src_gray, CV_BGR2GRAY );

        /// Reduce the noise so we avoid false circle detection
        //medianBlur ( src_gray, src_gray, 3 );

        vector<Vec3f> circles;

        /// Apply the Hough Transform to find the circles
        HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

        Point center;

        /// find the center of the circle detected
        if (circles.size() == 1)
        {
            center = Point2f(cvRound(circles[0][0]), cvRound(circles[0][1]));

          //  if (center.x < src_gray.cols*2/3 && center.x > src_gray.cols/3) {
                if (center.x > xMax) {
                    xMax = center.x;
                }
                if (center.x < xMin) {
                    xMin = center.x;
                }
          //  }
          //  if (center.y < src_gray.cols*2/3 && center.y > src_gray.cols/3) {
                if (center.y > yMax) {
                    yMax = center.y;
                }
                if (center.y < yMin) {
                    yMin = center.y;
                }
          //  }
            averagedCenter.x = (xMax + xMin)/2;
            averagedCenter.y = (yMax + yMin)/2;
        }
        /// circle center
        circle( src, averagedCenter, 3, Scalar(0,255,0), -1, 8, 0 );


        /// Show in a window
        std::string str("x: " + to_string((int)std::round(averagedCenter.x))
                        + " y: " + to_string((int)std::round(averagedCenter.y))
                        );
        putText(src, str.c_str(), cvPoint(30,30),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
        imshow( "Center Field", src );

        if (waitKey(5) == 27) { //wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }
    return 0;
}
