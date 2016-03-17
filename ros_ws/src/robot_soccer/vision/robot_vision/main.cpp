#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot_soccer/visiondata.h"
#include "robot_soccer/gameparam.h"

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
void gameStateCallback(const robot_soccer::gameparam &msg);



// takes an optional param file
int main(int argc, char** argv)
{
    loadConfigData(argc, argv);

    ros::init(argc, argv, "vision_data_pub");
    ros::NodeHandle n;
    ros::Publisher visionDataPub = n.advertise<robot_soccer::visiondata>("vision_data", 5);
    ros::Subscriber gameStateSub = n.subscribe("game_state", 1000, gameStateCallback);
    (void*)gameStateSub;


    Ball ball(config::ballArea);
    Robot robotAlly1;
    Robot robotAlly2;
    
    int windowDestroyTimer = 0;

    VideoCapture camera(config::cameraUrl);

    if (!camera.isOpened()) { // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    Mat imgLines;
    Point2f lastPos;
    if (config::trackRobot) {
        //Capture a temporary image from the camera
        Mat imgTmp;
        camera.read(imgTmp);

        //Create a black image with the size as the camera output
        imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );
    }

    cout << "starting video feed" << endl;

    while (ros::ok()) {
//        ros::Time timeStamp = ros::Time::now();

        Mat frame;

        if (!camera.read(frame)) { //if not success, break loop
            cout << "Cannot read frame from video stream" << endl;
            break;
        }

        robot_soccer::visiondata msg;
        msg.sys_time = ros::Time::now();
        Mat frameHSV;

        cvtColor(frame, frameHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        /// find our robots
        if (config::allyRobotCount > 0) {
            vector<cv::Moments> teamMoments = locateCvObjects(frameHSV, config::allyRobot1Color);
            robotAlly1.find(teamMoments);
            if (config::showVideo) {
                for (auto m: teamMoments) {
                    circle(frame, GetMomentCenter(m), 4, cvScalar(255,100,0), -1, 8, 0);
                }
            }
        }
        if (config::allyRobotCount > 1) {
            vector<cv::Moments> teamMoments = locateCvObjects(frameHSV, config::allyRobot2Color);
            robotAlly2.find(teamMoments);
            if (config::showVideo) {
                for (auto m: teamMoments) {
                    circle(frame, GetMomentCenter(m), 4, cvScalar(0,100,255), -1, 8, 0);
                }
            }

        }
        /// find the ball
        vector<cv::Moments> balls = locateCvObjects(frameHSV, config::ballColor);
        ball.find(balls);

        msg.tm0_x = robotAlly1.getCenter().x;
        msg.tm0_y = robotAlly1.getCenter().y;
        msg.tm0_w = robotAlly1.getRotation();

        msg.tm1_x = robotAlly2.getCenter().x;
        msg.tm1_y = robotAlly2.getCenter().y;
        msg.tm1_w = robotAlly2.getRotation();

        msg.ball_x = ball.getCenter().x;
        msg.ball_y = ball.getCenter().y;

        visionDataPub.publish(msg);


        if (config::trackRobot) {
            if (msg.tm0_x == msg.tm0_x && msg.tm0_x != 0.0) {

                //calculate the position of the robot
                Point2f pos = transformWorldFrametoCameraFrame(Point2f(msg.tm0_x, msg.tm0_y));
                if (lastPos.x == 0.0)
                    lastPos = pos;
                //Draw a red line from the previous point to the current point
                line(imgLines, pos, lastPos, Scalar(0,0,255), 2);
                lastPos = pos;
            } else {
                lastPos.x = 0.0;
            }
            frame = frame + imgLines;
        }

        if (config::showVideo) {
            std::string str(  " x: " + std::to_string(((int)std::round(msg.tm0_x)))
                            + " y: " + std::to_string(((int)std::round(msg.tm0_y)))
                            + " w: " + std::to_string(((int)std::round(msg.tm0_w)))
                            );
            putText(frame, str.c_str(), transformWorldFrametoCameraFrame(robotAlly1.getCenter())
                ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,50,250), 0.7, CV_AA);

            std::string str2(  "x: " + std::to_string(((int)std::round(msg.ball_x)))
                            + " y: " + std::to_string(((int)std::round(msg.ball_y)))
                            );
            putText(frame, str2.c_str(), Point(30,30)
                ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,50,250), 0.7, CV_AA);

            imshow("robot view", frame);
            windowDestroyTimer = 0;
            waitKey(1);
        } else if (windowDestroyTimer < 10) {
            windowDestroyTimer++;
            destroyWindow("robot view");
            waitKey(1);
        }

        ros::spinOnce();
//        cout << "time = " << (ros::Time::now() - timeStamp).toNSec() << endl;
    }
    
    return 0;
}

vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment)
{
    cv::Mat segment = ColorSegmentImage(frame, colorSegment);
    contour_vector_t contours = EdgeDetectImage(segment);
    return GetMoments(contours);
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

void gameStateCallback(const robot_soccer::gameparam& msg) {
    config::allyRobot1Color = getColorSpace(msg.ally1_color);
    config::allyRobot2Color = getColorSpace(msg.ally2_color);
    config::enemyRobot1Color = getColorSpace(msg.enemy1_color);
    config::enemyRobot2Color = getColorSpace(msg.enemy2_color);
    config::allyRobotCount = msg.ally_robot_count;
    config::enemyRobotCount = msg.enemy_robot_count;
    config::showVideo = msg.show_video;

    if (msg.field_pos == "home") {
        config::invertX = config::homeIsInverted;
    } else if (msg.field_pos == "away") {
        config::invertX = !config::homeIsInverted;
    }
}

// ros::Time timeStamp = ros::Time::now();
// cout << "time = " << (ros::Time::now() - timeStamp).toNSec() << endl;
