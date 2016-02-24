#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

RNG rng(12345);

 int main( int argc, char** argv )
 {
    VideoCapture cap("http://192.168.1.78:8080/stream?topic=/image&dummy=param.mjpg"); //capture the video from web cam

    if (!cap.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 200;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    vector<float> values(10);


    while (true) {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//        imgOriginal = imread(argv[1], 1);
//        if ( !imgOriginal.data )
//        {
//            printf("No image data \n");
//            return -1;
//        }

        if (!bSuccess) { //if not success, break loop
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        imshow("Original", imgOriginal); //show the original image

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

        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image

        int thresh = 100;
        int max_thresh = 255;

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

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

        /// Draw contours
        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            Scalar color = Scalar( 100, 200, 150);
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            circle( drawing, mc[i], 4, color, -1, 8, 0 );
            std::string str(  " A: " + std::to_string(mu[i].m00));
            putText(drawing, str.c_str(), mc[i]
                ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,250), 0.7, CV_AA);
        }

          /// Show in a window
          imshow( "Contours", drawing);


        if (waitKey(5) == 27) { //wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    return 0;

}
