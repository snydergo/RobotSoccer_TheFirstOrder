#include <cstdio>
#include <iostream>
#include <string>

#include <ros/callback_queue.h>

#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"
#include "debug/subscriber_debugmsg.h"
#include "kalmanfilter/getFilteredData.h"
#include "gameplay/strategy.h"
#include "types.h"

#include "std_msgs/String.h"

#define OPTION 1
#define XCMD 2
#define YCMD 3
#define THETACMD 4

void gameCmdCallback(const std_msgs::String &msg);

void checkCmd(robot_soccer::controldata &cmdRob1);
void debugOption();

void mainControlSM(ros::NodeHandle &n)
{
    //PUBLISHER FOR MOTIONCONTROL
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 5);
    ros::Publisher robo2Com = n.advertise<robot_soccer::controldata>("robot2Com", 5);

    //SUBSCRIBER FROM VISION
    ros::Subscriber vision_subscriber = n.subscribe("vision_data", 5, visionCallback);
    (void*)vision_subscriber;

    ros::Rate loop_rate(TICKS_PER_SEC);
    int count = 0;

    bool dataInitialized = false;

    //    ros::Subscriber filter_subscriber = n.subscribe("outputfilter", 1000, filterCallback);
    //    ros::Publisher filterCom = n.advertise<robot_soccer::visiondata>("inputfilter", 1000);

    ros::Subscriber gameCmdSub = n.subscribe("game_cmd", 5, gameCmdCallback);
    (void*)gameCmdSub;
    Strategies strategies;
    strategies.init();
    while (ros::ok())
    {
        count++;
        if (visionUpdated) {
            visionUpdated = false;
           // filterCom.publish(robot_soccer::visiondata(visionStatus_msg));
            dataInitialized = true;
            field.updateStatus(visionStatus_msg);
        }
        strategies.tick();

        if (sendCmd_Rob1) {
            sendCmd_Rob1 = false;
            checkCmd(cmdRob1);
            robo1Com.publish(cmdRob1);
        }
        if (sendCmd_Rob2){
            sendCmd_Rob2 = false;
            checkCmd(cmdRob2);
            robo2Com.publish(cmdRob2);
        }

        ros::spinOnce();

        loop_rate.sleep();

    }
}

void debugSM(ros::NodeHandle &n)
{
    //PUBLISHER FOR MOTIONCONTROL
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 5);

    //SUBSCRIBER FROM VISION
    ros::Subscriber vision_subscriber = n.subscribe("vision_data", 5, visionCallback);
    (void*)vision_subscriber;

    ros::Rate loop_rate(TICKS_PER_SEC);
    bool dataInitialized = false;
    bool init = true;

    ros::Subscriber debug_subscriber = n.subscribe("debug", 5, debugCallback);
    (void*)debug_subscriber;
    Skills skill1(robotType::ally1);
    skill1.init();
    Point dest;
    bool kickball = false;
    char kickCounter = 0;
    bool kickCmd = false;
    while(ros::ok()){

        if (visionUpdated) {
            visionUpdated = false;
            dataInitialized = true;
            field.updateStatus(visionStatus_msg);
        }
        if (newDebugCmd || init) {
            init = false;
            newDebugCmd = false;
            cmdRob1.cmdType = debugCmd.cmdType;
            cmdRob1.x_cmd = debugCmd.x_cmd;
            cmdRob1.y_cmd = debugCmd.y_cmd;
            cmdRob1.theta_cmd = debugCmd.theta_cmd;

            if (cmdRob1.cmdType == "move") {
                dest = Point(cmdRob1.x_cmd,cmdRob1.y_cmd);
                skill1.goToPoint(dest, cmdRob1.theta_cmd);
            } else if (cmdRob1.cmdType == "kick") {
                kickball = true;
                skill1.kick();
            } else if (cmdRob1.cmdType == "kickinit"){
                skill1.init_kick();
                std::cout << "initing kicker" << std::endl;
            } else if (cmdRob1.cmdType == "kickuninit"){
                std::cout << "unit kicker" << std::endl;
                skill1.uninit_kick();
            } else{
                skill1.idle();
            }
        }

        skill1.tick();
        if (sendCmd_Rob1) {
            sendCmd_Rob1 = false;
            checkCmd(cmdRob1);
            std::cout << "cmdType: "+ cmdRob1.cmdType << std::endl << "x: " << cmdRob1.x_cmd << " y: " << cmdRob1.y_cmd << " w: " << cmdRob1.theta_cmd << std::endl;
            robo1Com.publish(cmdRob1);

        }
        ros::spinOnce();

        loop_rate.sleep();
    }
}

void filterSM(ros::NodeHandle &n)
{
    bool dataInitialized = false;
    ros::Rate loop_rate(TICKS_PER_SEC);
    ros::Subscriber filter_subscriber = n.subscribe("outputfilter", 5, filterCallback);
    (void*)filter_subscriber;
    ros::Publisher filterCom = n.advertise<robot_soccer::visiondata>("inputfilter", 5);
    while(ros::ok()){
        if (visionUpdated) {
           visionUpdated = false;
           dataInitialized = true;
           field.updateStatus(visionStatus_msg);
           filterCom.publish(vision_msg);
        }
        if (predictedUpdated){
            std::cout << "robot_x == " << std::to_string(predicted.ally1.location.x) << std::endl
                         << "robot_y == " << std::to_string(predicted.ally1.location.y) << std::endl
                         << "robot_theta == " << std::to_string(predicted.ally1.theta) << std::endl;
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
}

void predictSM(ros::NodeHandle &n)
{
    //PUBLISHER FOR MOTIONCONTROL
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 5);
    ros::Publisher robo2Com = n.advertise<robot_soccer::controldata>("robot2Com", 5);

    //SUBSCRIBER FROM FILTERED VISION
    ros::Subscriber vision_subscriber = n.subscribe("filteredvision_data", 5, filterCallback);
    (void*)vision_subscriber;

    ros::Rate loop_rate(TICKS_PER_SEC);
    int count = 0;

    bool dataInitialized = false;

//    ros::Subscriber filter_subscriber = n.subscribe("outputfilter", 1000, filterCallback);
//    ros::Publisher filterCom = n.advertise<robot_soccer::visiondata>("inputfilter", 1000);

    ros::Subscriber gameCmdSub = n.subscribe("game_cmd", 5, gameCmdCallback);
    (void*)gameCmdSub;
    Strategies strategies;
    strategies.init();
    while (ros::ok())
    {
        count++;
        if (predictedUpdated) {
            predictedUpdated = false;
           // filterCom.publish(robot_soccer::visiondata(visionStatus_msg));
            dataInitialized = true;
            field.updateStatus(predicted);
        }
        strategies.tick();

        if (sendCmd_Rob1) {
            sendCmd_Rob1 = false;
            checkCmd(cmdRob1);
            robo1Com.publish(cmdRob1);
        }
        if (sendCmd_Rob2){
            sendCmd_Rob2 = false;
            checkCmd(cmdRob2);
            robo2Com.publish(cmdRob2);
        }

        ros::spinOnce();

        loop_rate.sleep();

    }
}

int main(int argc, char *argv[])
{

    std::string option;
    try{
        option = std::string(argv[OPTION]);
    }catch(...){
            option = "none";
    }

    //need to perform all necessary inits
    ros::init(argc, argv, "mainhub");
    ros::NodeHandle n;

    if (option == "debug") {
        debugSM(n);
    } else if (option == "filter") {
        filterSM(n);
    } else if(option == "predict"){
        predictSM(n);
    } else {
        mainControlSM(n);
    }
    return 0;
}

void gameCmdCallback(const std_msgs::String &msg)
{
    if (msg.data == "stop") {
        /// ----------------- send stop command to robot -------------------
    } else if (msg.data == "start") {
        /// ----------------- start the AI state machine -------------------
    } else if (msg.data == "mark") {
        /// ----------------- go to starting postion for match -------------
    } else {
        std::cout << "ERROR: Unknown game command: " << msg << std::endl;
    }
}



void checkCmd(robot_soccer::controldata &cmdRob){
    if(cmdRob.x != cmdRob.x || cmdRob.x_cmd != cmdRob.x_cmd){
        std::cout << "maincontrol::checkCmd() ##nan's detected## reverting to IDLE" << std::endl;
        cmdRob.cmdType = "idle";

    }
}

void debugOption(){

}
