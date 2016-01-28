#include "objectdetection.h"


using namespace cv;

void CreateControlWindow(const HsvColorSubSpace& colorSegment)
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &colorSegment.hue.low, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &colorSegment.hue.high, 179);

    cvCreateTrackbar("LowS", "Control", &colorSegment.saturation.low, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &colorSegment.saturation.high, 255);

    cvCreateTrackbar("LowV", "Control", &colorSegment.value.low, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &colorSegment.value.high, 255);
}

VideoCapture ConnectCamera(std::string uri)
{
    VideoCapture cap((uri == "default") ? "http://192.168.1.10:8080/stream?topic=/image&dummy=param.mjpg" : uri); //capture the video from web cam

    if (!cap.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    return cap;
}

Mat ReadFrame(VideoCapture camera)
{
    Mat img;
    bool bSuccess = camera.read(img); // read a new frame from video

    if (!bSuccess) { //if not success, break loop
        cout << "Cannot read a frame from video stream" << endl;
        break;
    }
    Mat imgHSV;

    cvtColor(img, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    return imgHSV;
}

Mat ColorSegmentImage(const Mat& src, const HsvColorSubSpace& colorSegment)
{
    Mat imgThresholded;

    inRange(src, Scalar(colorSegment.hue.low, colorSegment.saturation.low, colorSegment.value.low),
                 Scalar(colorSegment.hue.high, colorSegment.saturation.high, colorSegment.value.high),
                 imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    erode( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}

contour_vector_t EdgeDetectImage(const Mat& src)
{
    int thresh = 100;

    Mat canny_output;
    contour_vector_t contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny(src, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    return contours;
}

vector<Moments> GetMoments(contour_vector_t contours)
{
    vector<Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }
    return mu;
}

Point2f GetMomentCenter(Moments moments)
{
    ///  Get the mass centers:
    vector<Point2f> mc( moments.size() );
    for( int i = 0; i < moments.size(); i++ )
    { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

}

vector<double> GetMomentArea(Moments moments)
{
    vector<double> mc( moments.size() );
    for( int i = 0; i < moments.size(); i++ )
    double dArea = oMoments.m00;

}
