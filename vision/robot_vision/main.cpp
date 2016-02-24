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

using namespace std;
using namespace cv;

void loadConfigData(int argc, char** argv);
vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment);
Point2f trasformCameraFrameToWorldFrame(Point2f point);

// takes an optional param file
int main(int argc, char** argv)
{
    loadConfigData(argc, argv);
    ros::init(argc, argv, "vision_data_pub");
    ros::NodeHandle n;
    ros::Publisher visionDataPub = n.advertise<robot_soccer::visiondata>("vision_data", 5);
    Ball ball(config::ballArea);
    
    int iLastX;
    int iLastY;
    bool detectedLastFrame;

    VideoCapture camera(config::cameraUrl);

    if (!camera.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

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

        vector<UndefinedCVObject> uObjects;
        for (int i = 0; i < teamMoments.size(); i+=2) {
            UndefinedCVObject obj(teamMoments[i]);
            uObjects.push_back(obj);
        }
        //find the ball
        vector<cv::Moments> balls = locateCvObjects(frameHSV, config::ballColor);


        vector<UndefinedCVObject> ballObjects;
        for (int i = 0; i < balls.size(); i+=2) {
            UndefinedCVObject obj(balls[i]);
            ballObjects.push_back(obj);
        }

        ball.find(ballObjects);

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

        if (msg.tm0_x == msg.tm0_x && msg.tm0_x != 0.0) {
            //calculate the position of the robot
            int posX = transformWorldFrametoCameraFrame(msg.tm0_x);
            int posY = transformWorldFrametoCameraFrame(msg.tm0_y);        
                
            //Draw a red line from the previous point to the current point
            Scalar color;
            if (detectedLastFrame) {
                color = (0,0,255);
            } else {
                color = (255,0,0);
            }
            line(frame, Point(posX, posY), Point(iLastX, iLastY), color, 2);
            
            detectedLastFrame = true;
            iLastX = posX;
            iLastY = posY;
        } else {
            detectedLastFrame = false;
        }
        
        visionDataPub.publish(msg);

        imshow("robot view", frame);
        waitKey(1);
    }
    
    return 0;
}

Point2f trasformCameraFrameToWorldFrame(Point2f point)
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

Point2f transformWorldFrametoCameraFrame(Point2f point)
{
    Point2f retVal;
    retVal.x = (point.x / config::cmPerPixelConversionFactor) + config::fieldCenter_px.x;
    retVal.y = (point.y / config::cmPerPixelConversionFactor) + config::fieldCenter_px.y;
    if (config::invertX) {
        retVal.x = -retVal.x;
    } else {
        retVal.y = -retVal.y;
    }
    return retVal;
}

vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment)
{
    cv::Mat segment = ColorSegmentImage(frame, colorSegment);
    contour_vector_t contours = EdgeDetectImage(segment);
    return GetMoments(contours);
}


