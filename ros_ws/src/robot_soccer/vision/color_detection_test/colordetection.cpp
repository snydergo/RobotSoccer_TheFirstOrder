#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
     string cameraUrl = "http://192.168.1.79:8080/stream?topic=/image&dummy=param.mjpg";
     if (argc == 2) {
         try {
             int ip;
             ip = stoi(argv[1]);
             if (ip == 78) {
                 cout << "loading camera 192.168.1.78..." << endl;
                 cameraUrl = "http://192.168.1.78:8080/stream?topic=/image&dummy=param.mjpg";
             } else {
                 cout << "loading camera 192.168.1.79..." << endl;
             }
         } catch(...) {}
     }
    VideoCapture cap(cameraUrl); //capture the video from web cam

    if (!cap.isOpened()) { // if not success, exit program 
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0; 
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
//    Mat img;
//    cap.read(img); // read a new frame from video
//    vector<int> compression_params;
//    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//    compression_params.push_back(9);

//    try {
//        imwrite("image.png", img, compression_params);
//    }
//    catch (runtime_error& ex) {
//        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
//        return 1;
//    }

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

        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image

        if (waitKey(10) == 27) { //wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

    return 0;

}
