#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot_soccer/visiondata.h"

#include "parameters.h"
#include "hsvcolorsubspace.h"
#include "utils.h"
#include "objectdetection.h"
#include "config.h"
#include "undefinedcvobject.h"
#include "robot.h"
#include "ball.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

void loadConfigData(int argc, char** argv);
vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment);
Point2f trasformCameraFrameToWorldFrame(const Point2f point);
Point2f transformWorldFrametoCameraFrame(const Point2f point);
vector<UndefinedCVObject> getCVObjects(vector<cv::Moments> moments);

Mat frame;
mutex frameMtx;


// takes an optional param file
int main(int argc, char** argv)
{
    loadConfigData(argc, argv);

    ros::init(argc, argv, "vision_data_pub");
    ros::NodeHandle n;
    ros::Publisher visionDataPub = n.advertise<robot_soccer::visiondata>("vision_data", 5);
    Ball ball(config::ballArea);
    Robot robotAlly1;
    
    Point2f lastPos;

    VideoCapture camera(config::cameraUrl);

    if (!camera.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

//    //Capture a temporary image from the camera
//    Mat imgTmp;
//    camera.read(imgTmp);

//    //Create a black image with the size as the camera output
//    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );



    cout << "starting video feed" << endl;
    while (ros::ok()) {
        Mat frame;

        if (!camera.read(frame)) { //if not success, break loop
            cout << "Cannot read frame from video stream" << endl;
            break;
        }
        Mat frameHSV;

        cvtColor(frame, frameHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        /// find our robots
        vector<cv::Moments> teamMoments = locateCvObjects(frameHSV, config::teamRobotPrimaryColor);

        for (auto m: teamMoments) {
            circle(frame, GetMomentCenter(m), 4, cvScalar(255,100,0), -1, 8, 0);
        }

        vector<UndefinedCVObject> uObjects = getCVObjects(teamMoments);
        //find the ball
        vector<cv::Moments> balls = locateCvObjects(frameHSV, config::ballColor);


        vector<UndefinedCVObject> ballObjects;
        for (int i = 0; i < balls.size(); i+=2) {
            UndefinedCVObject obj(balls[i]);
            ballObjects.push_back(obj);
        }

        ball.find(ballObjects);
        robotAlly1.find(uObjects);

        Moments rear;
        Moments front;
        rear.m00 = 0;
        front.m00 = 0;
        for (int i = 0; i < teamMoments.size(); i+=2) {
            if (GetMomentArea(rear) <= GetMomentArea(teamMoments[i])) {
                front = rear;
                rear = teamMoments[i];
            } else if (GetMomentArea(front) <= GetMomentArea(teamMoments[i]) ) {
                front = teamMoments[i];
            }
        }

        robot_soccer::visiondata msg;
        msg.sys_time = ros::Time::now();

        Point2f frontCenter = trasformCameraFrameToWorldFrame(GetMomentCenter(front));
        Point2f rearCenter = trasformCameraFrameToWorldFrame(GetMomentCenter(rear));

        msg.tm0_x = ((frontCenter.x + rearCenter.x)/2);
        msg.tm0_y = ((frontCenter.y + rearCenter.y)/2);

        msg.tm0_w = angleFrom(rearCenter, frontCenter);

        msg.ball_x = ball.getCenter().x;
        msg.ball_y = ball.getCenter().y;

        std::string str(  "x: " + std::to_string(((int)std::round(msg.tm0_x)))
                        + " y: " + std::to_string(((int)std::round(msg.tm0_y)))
                        + " w: " + std::to_string(((int)std::round(msg.tm0_w)))
                        );
        putText(frame, str.c_str(), GetMomentCenter(front)
            ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,50,250), 0.7, CV_AA);

        std::string str2(  "x: " + std::to_string(((int)std::round(msg.ball_x)))
                        + " y: " + std::to_string(((int)std::round(msg.ball_y)))
                        );
        putText(frame, str2.c_str(), Point(30,30)
            ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,50,250), 0.7, CV_AA);
//        if (config::trackRobot) {
//            if (msg.tm0_x == msg.tm0_x && msg.tm0_x != 0.0) {

//                //calculate the position of the robot
//                Point2f pos = transformWorldFrametoCameraFrame(Point2f(msg.tm0_x, msg.tm0_y));
//                if (lastPos.x == 0.0)
//                    lastPos = pos;
//                //Draw a red line from the previous point to the current point
//                line(imgLines, pos, lastPos, Scalar(0,0,255), 2);
//                lastPos = pos;
//            } else {
//                lastPos.x = 0.0;
//            }
//        }
        
        visionDataPub.publish(msg);
        //frame = frame + imgLines;
        imshow("robot view", frame);
        waitKey(1);
    }
    
    return 0;
}

vector<UndefinedCVObject> getCVObjects(vector<cv::Moments> moments)
{
    vector<UndefinedCVObject> retObjs;
    for (int i = 0; i < moments.size(); i+=2) {
        UndefinedCVObject obj(moments[i]);
        retObjs.push_back(obj);
    }
    return retObjs;
}


void listenCameraFeed()
{
    VideoCapture camera(config::cameraUrl);

    if (!camera.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return;
    }
    cout << "starting video feed" << endl;
    while (ros::ok()) {
        frameMtx.lock();
        if (!camera.read(frame)) { //if not success, break loop
            cout << "Cannot read frame from video stream" << endl;
            break;
        }
        frameMtx.unlock();
    }

}

void processFrame()
{
    while (ros::ok()) {
        Mat frameHSV;
        frameMtx.lock();
        cvtColor(frame, frameHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        frameMtx.unlock();
        /// find our robots
        vector<cv::Moments> teamMoments = locateCvObjects(frameHSV, config::teamRobotPrimaryColor);
        for (auto m: teamMoments) {
            circle(frame, GetMomentCenter(m), 4, cvScalar(255,100,0), -1, 8, 0);
        }
    }

}

void processObjects()
{
}

// LPF: Y(n) = Y(n-1) - (ß*(Y(n-1)-X(n)));
void lowPassFilter(const Point2f &rawData, Point2f &filterData)
{
    static const float beta = 0.5;
    filterData = filterData - (beta * (filterData - rawData));
}





Point2f trasformCameraFrameToWorldFrame(const Point2f point)
{
    Point2f retVal;
    retVal.x = (point.x - config::fieldCenter_px.x) * config::cmPerPixelConversionFactor;
    retVal.y = (point.y - config::fieldCenter_px.y) * config::cmPerPixelConversionFactor;
    if (config::invertX) {
        retVal.x = -retVal.x;
    } else {
        retVal.y = -retVal.y;
    }
    return retVal;
}

Point2f transformWorldFrametoCameraFrame(const Point2f point)
{
    Point2f retVal = point;
    if (config::invertX) {
        retVal.x = -retVal.x;
    } else {
        retVal.y = -retVal.y;
    }
    retVal.x = (retVal.x / config::cmPerPixelConversionFactor) + config::fieldCenter_px.x;
    retVal.y = (retVal.y / config::cmPerPixelConversionFactor) + config::fieldCenter_px.y;

    return retVal;
}

vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment)
{
    cv::Mat segment = ColorSegmentImage(frame, colorSegment);
    contour_vector_t contours = EdgeDetectImage(segment);
    return GetMoments(contours);
}

/*
#include <cv_bridge/cv_bridge.h>
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        Mat frame = cv_bridge::toCvShare(msg, "bgr8")->image;
        processImage(frame);
        imshow("Soccer Overhead Camera", frame);        
        waitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "vision");
    ros::NodeHandle nh;

    // Subscribe to camera
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber image_sub = it.subscribe("/camera1/image_raw", 1, imageCallback);

    ros::init(argc, argv, "vision_data_pub");
    ros::NodeHandle n;
    ros::Publisher visionDataPub = n.advertise<robot_soccer::visiondata>("vision_data", 5);

    // Publish robot locations
    //soccer_pub = nh.advertise<walle::SoccerPoses>("/vision", 5);
    ros::spin();
    return 0;
}
*/
