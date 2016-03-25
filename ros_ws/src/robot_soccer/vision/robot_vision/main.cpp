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
#include <signal.h>

using namespace std;
using namespace cv;

void loadConfigData(int argc, char** argv);
vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment);
Point2f trasformCameraFrameToWorldFrame(const Point2f point);
Point2f transformWorldFrametoCameraFrame(const Point2f point);
vector<UndefinedCVObject> getCVObjects(vector<cv::Moments> moments);
void gameStateCallback(const robot_soccer::gameparam &msg);

enum class FieldObject {
    ball,
    allyRobot1,
    allyRobot2,
    enemyRobot1,
    enemyRobot2
};

//thread ally2Thread;
//thread enemy1Thread;
//thread enemy2Thread;


vector<bool> signallers = {false, false, false, false, false};
vector<vector<cv::Moments>> objectMoments(5);

HsvColorSubSpace getColor(FieldObject obj) {
    switch (obj) {
    case FieldObject::ball:        return config::ballColor;
    case FieldObject::allyRobot1:  return config::allyRobot1Color;
    case FieldObject::allyRobot2:  return config::allyRobot2Color;
    case FieldObject::enemyRobot1: return config::enemyRobot1Color;
    case FieldObject::enemyRobot2: return config::enemyRobot2Color;
    }
}

bool getSignaller(FieldObject obj) {
    switch (obj) {
    case FieldObject::ball:        return signallers[0];
    case FieldObject::allyRobot1:  return signallers[1];
    case FieldObject::allyRobot2:  return signallers[2];
    case FieldObject::enemyRobot1: return signallers[3];
    case FieldObject::enemyRobot2: return signallers[4];
    }
}

void setSignaller(FieldObject obj, bool value) {
    switch (obj) {
    case FieldObject::ball:        signallers[0] = value; return;
    case FieldObject::allyRobot1:  signallers[1] = value; return;
    case FieldObject::allyRobot2:  signallers[2] = value; return;
    case FieldObject::enemyRobot1: signallers[3] = value; return;
    case FieldObject::enemyRobot2: signallers[4] = value; return;
    }
}

//bool waitForThreads()
//{
//    while (signallers[0] ||
//           signallers[1] ||
//           signallers[2] ||
//           signallers[3] ||
//           signallers[4]){}


//}

vector<cv::Moments> getMoments(FieldObject obj) {
    switch (obj) {
    case FieldObject::ball:        return objectMoments[0];
    case FieldObject::allyRobot1:  return objectMoments[1];
    case FieldObject::allyRobot2:  return objectMoments[2];
    case FieldObject::enemyRobot1: return objectMoments[3];
    case FieldObject::enemyRobot2: return objectMoments[4];
    }
}

void setMoments(FieldObject obj, vector<cv::Moments> value) {
    switch (obj) {
    case FieldObject::ball:        objectMoments[0] = value; return;
    case FieldObject::allyRobot1:  objectMoments[1] = value; return;
    case FieldObject::allyRobot2:  objectMoments[2] = value; return;
    case FieldObject::enemyRobot1: objectMoments[3] = value; return;
    case FieldObject::enemyRobot2: objectMoments[4] = value; return;
    }
}

Mat frameHSV;
bool terminateThreads = false;

void processFrames(FieldObject obj)
{
    while (ros::ok()) {
        if (getSignaller(obj)) {
            setMoments(obj, locateCvObjects(frameHSV, getColor(obj)));
            setSignaller(obj, false);
        }
    }

}

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

    thread ballThread(processFrames, FieldObject::ball);
    thread ally1Thread(processFrames, FieldObject::allyRobot1);
    thread ally2Thread(processFrames, FieldObject::allyRobot2);

    cout << "starting video feed" << endl;

    while (ros::ok()) {
//        ros::Time timeStamp = ros::Time::now();

        Mat frame;

        if (!camera.read(frame)) { //if not success, break loop
            cout << "Cannot read frame from video stream" << endl;
            break;
        }

        robot_soccer::visiondata msg;
        msg.sys_time = ros::Time::now(); // get the current time stamp

        cvtColor(frame, frameHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        setSignaller(FieldObject::ball, true);
        if (config::allyRobotCount > 0) {
            setSignaller(FieldObject::allyRobot1, true);
        }
        if (config::allyRobotCount > 1) {
            setSignaller(FieldObject::allyRobot2, true);
        }

        /// find our robots
        if (config::allyRobotCount > 0) {
            robotAlly1.find(getMoments(FieldObject::allyRobot1));
        }
        if (config::allyRobotCount > 1) {
            robotAlly2.find(getMoments(FieldObject::allyRobot2));
        }
        /// find the ball
        ball.find(getMoments(FieldObject::ball));

        msg.tm0_x = robotAlly1.getCenter().x;
        msg.tm0_y = robotAlly1.getCenter().y;
        msg.tm0_w = robotAlly1.getRotation();

        msg.tm1_x = robotAlly2.getCenter().x;
        msg.tm1_y = robotAlly2.getCenter().y;
        msg.tm1_w = robotAlly2.getRotation();

        msg.ball_x = ball.getCenter().x;
        msg.ball_y = ball.getCenter().y;

        visionDataPub.publish(msg);

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
    ballThread.join();
    ally1Thread.join();
    ally2Thread.join();

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
