#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot_soccer/visiondata.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "hsvcolorsubspace.h"
#include "utils.h"
#include "objectdetection.h"
#include "config.h"
#include "undefinedcvobject.h"
#include "robot.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <math.h>

using namespace rapidjson;
using namespace std;
using namespace cv;

void loadConfigData(char** argv);
vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment);
Point2f trasformCameraFrameToWorldFrame(Point2f point);

int main(int argc, char** argv)
{
    if (argc != 2) { cout << "no param file. Usage: program [param.json]\n"; return 1; }
    cout << "loading param file..." << endl;
    loadConfigData(argv);
    cout << "param file successfully loaded" << endl;
    ros::init(argc, argv, "vision_data_pub");
    ros::NodeHandle n;
    ros::Publisher visionDataPub = n.advertise<robot_soccer::visiondata>("vision_data", 5);
    // ros::Rate loop_rate(30);
    bool robotUpdated = false;
    Robot robot;

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
        // find their robots
        //vector<cv::Moments> opponetMoments = locateCvObjects(frame, config::opponentRobotPrimaryColor);
        //find the ball
        //vector<cv::Moments> balls = locateCvObjects(frame, config::ballColor);
        for (auto m: teamMoments) {
            circle(frame, GetMomentCenter(m), 4, cvScalar(255,100,0), -1, 8, 0);
        }

        vector<UndefinedCVObject> uObjects;
        int j = 0;
        for (int i = 0; i < teamMoments.size(); i+=2) {
            UndefinedCVObject obj(teamMoments[i], config::teamRobotPrimaryColor);
            uObjects.push_back(obj);
        }



        robotUpdated = robot.find(uObjects);




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

        std::string str(  "x: " + std::to_string(((int)std::round(msg.tm0_x)))
                        + " y: " + std::to_string(((int)std::round(msg.tm0_y)))
                        + " w: " + std::to_string(((int)std::round(msg.tm0_w)))
                        );
        putText(frame, str.c_str(), GetMomentCenter(front)
            ,FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 0.7, CV_AA);


        visionDataPub.publish(msg);
        //ros::spinOnce();
        //loop_rate.sleep();
        imshow("robot view", frame);
        waitKey(5);
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

void loadConfigData(char** argv)
{
    FILE* file = fopen(argv[1], "r");
    char buffer[65536];
    FileReadStream is(file, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);
    Value& root = document["param"];

    config::cameraUrl = root["cameraUrl"].GetString();

    Value& calibrate = root["calibration"];

    config::fieldCenter_px.x = calibrate["fieldCenterX_pixel"].GetDouble();
    config::fieldCenter_px.y = calibrate["fieldCenterY_pixel"].GetDouble();

    config::cmPerPixelConversionFactor = calibrate["referenceDistance_cm"].GetDouble() /
                                         calibrate["referenceDistance_pixel"].GetDouble();
    
    config::invertX = calibrate["invertDirectionX"].GetBool();
    
    Value& crop = root["crop"];

    config::cropLeft = crop["left"].GetInt();
    config::cropRight = crop["right"].GetInt();
    config::cropTop = crop["top"].GetInt();
    config::cropBottom = crop["bottom"].GetInt();
    
    Value& ballColorHsv = root["ball"]["color"];
    
    config::ballColor.hue.low         = ballColorHsv["lowH"].GetInt();
    config::ballColor.hue.high        = ballColorHsv["highH"].GetInt();
    config::ballColor.saturation.low  = ballColorHsv["lowS"].GetInt();
    config::ballColor.saturation.high = ballColorHsv["highS"].GetInt();
    config::ballColor.value.low       = ballColorHsv["lowV"].GetInt();
    config::ballColor.value.high      = ballColorHsv["highV"].GetInt();

    config::teamRobotCount = root["teamRobots"]["count"].GetInt();
    
    Value& teamRobotPrimary = root["teamRobots"]["color"];
    config::teamRobotPrimaryColor.hue.low         = teamRobotPrimary["lowH"].GetInt();
    config::teamRobotPrimaryColor.hue.high        = teamRobotPrimary["highH"].GetInt();
    config::teamRobotPrimaryColor.saturation.low  = teamRobotPrimary["lowS"].GetInt();
    config::teamRobotPrimaryColor.saturation.high = teamRobotPrimary["highS"].GetInt();
    config::teamRobotPrimaryColor.value.low       = teamRobotPrimary["lowV"].GetInt();
    config::teamRobotPrimaryColor.value.high      = teamRobotPrimary["highV"].GetInt();

    config::opponentRobotCount = root["opponentRobots"]["count"].GetInt();
    
    Value& opponentRobotPrimary = root["opponentRobots"]["color"];
    config::opponentRobotPrimaryColor.hue.low         = opponentRobotPrimary["lowH"].GetInt();
    config::opponentRobotPrimaryColor.hue.high        = opponentRobotPrimary["highH"].GetInt();
    config::opponentRobotPrimaryColor.saturation.low  = opponentRobotPrimary["lowS"].GetInt();
    config::opponentRobotPrimaryColor.saturation.high = opponentRobotPrimary["highS"].GetInt();
    config::opponentRobotPrimaryColor.value.low       = opponentRobotPrimary["lowV"].GetInt();
    config::opponentRobotPrimaryColor.value.high      = opponentRobotPrimary["highV"].GetInt();
    
    fclose(file);

}
