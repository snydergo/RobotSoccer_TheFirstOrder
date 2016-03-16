#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
    VideoCapture cap("http://192.168.1.78:8080/stream?topic=/image&dummy=param.mjpg"); //capture the video from web cam

    if (!cap.isOpened()) { // if not success, exit program 
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 60;// calibrated for green construction paper
    int iHighH = 90;

    int iLowS = 30;
    int iHighS = 145;

    int iLowV = 130;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    while (true) {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) { //if not success, break loop
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        int thresh = 100;

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        imshow("Original", imgOriginal); //show the original image

        /// Detect edges using canny
        Canny( imgThresholded, canny_output, thresh, thresh*2, 3 );
        /// Find contours
        findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        /// Get the moments
          vector<Moments> mu(contours.size() );
          for( int i = 0; i < contours.size(); i++ )
         { mu[i] = moments( contours[i], false ); }

          ///  Get the mass centers:
          vector<Point2f> mc( contours.size() );
          for( int i = 0; i < contours.size(); i++ )
         { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }


        double distance = 0;
        if (mc.size() == 4) {
            distance = std::sqrt((std::pow((mc[0].x-mc[2].x), 2) + std::pow((mc[0].y-mc[2].y), 2)));
        }

        /// Draw contours
        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( 100, 200, 150);
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
            circle(drawing, mc[i], 4, color, -1, 8, 0 );
        }

        /// Show in a window
        std::string str("distance in pixels: " + std::to_string(((int)std::round(distance))));
        putText(drawing, str.c_str(), cvPoint(30,30),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
        imshow( "Contours", drawing);

        if (waitKey(5) == 27) { //wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

    return 0;

}
