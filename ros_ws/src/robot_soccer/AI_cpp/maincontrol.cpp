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
#include "termios.h"

#define OPTION 1
#define XCMD 2
#define YCMD 3
#define THETACMD 4

void checkCmd(robot_soccer::controldata &cmdRob1);
void debugOption();

int main(int argc, char *argv[])
{
    //need to perform all necessary inits
    ros::init(argc, argv, "mainhub");
    ros::NodeHandle n;

    //SUBSCRIBER FROM VISION
    ros::Subscriber vision_subscriber = n.subscribe("vision_data", 1000, visionCallback);
    (void*)vision_subscriber;

    //PUBLISHER FOR MOTIONCONTROL
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 1000);
    ros::Publisher robo2Com = n.advertise<robot_soccer::controldata>("robot2Com", 1000);
    ros::Rate loop_rate(TICKS_PER_SEC);
    int count = 0;

    bool dataInitialized = false;
    std::string option;
    try{
        option = std::string(argv[OPTION]);
    }catch(...){
            option = "none";
    }
    
    //std::string x(argv[XCMD]);
    //std::string y(argv[YCMD]);
    //std::cout << option + " x = " + x + " y = " + y << std::endl;
    ///### DEBUG OPTION ###///
    bool init = true;
    if (option == "debug"){
        ros::Subscriber debug_subscriber = n.subscribe("debug", 1000, debugCallback);
        (void*)debug_subscriber;
        Skills skill1(robotType::ally1);
        skill1.init();
        Point dest;
        bool kickball = false;
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
//                    if (!bkcalc::atLocation(robotType::ally1, dest)) {
                    skill1.goToPoint(dest, cmdRob1.theta_cmd);
//                    } else {
//                        skill1.idle();
//                    }
                } else if (cmdRob1.cmdType == "kick") {
                    kickball = true;
                    skill1.kick();
                } else {
                    skill1.idle();
                }
            }

            skill1.tick();
            if (sendCmd_Rob1) {
                sendCmd_Rob1 = false;
                checkCmd(cmdRob1);
                std::cout << "x: " << cmdRob1.x_cmd << " y: " << cmdRob1.y_cmd << " w: " << cmdRob1.theta_cmd << std::endl;
                robo1Com.publish(cmdRob1);

            }
            ros::spinOnce();

            loop_rate.sleep();
        }
        return 0;
    //### FILTER TEST OPTION ###//
    }else if(option == "filter"){
        ros::Subscriber filter_subscriber = n.subscribe("outputfilter", 1000, filterCallback);
        ros::Publisher filterCom = n.advertise<robot_soccer::visiondata>("inputfilter", 1000);
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
        return 0;
    }

    ///### NORMAL MAINCONTROL ###///

    //strategies STATEMACHINE
    Strategies strategies;
    strategies.init();
    while (ros::ok())
    {
        count++;
        if (visionUpdated) {
            visionUpdated = false;
            dataInitialized = true;
            /*//std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                        visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;*/
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

        //ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    std::cout << "returning" << std::endl;
    return 0;
}

void checkCmd(robot_soccer::controldata &cmdRob){
    if(cmdRob.x != cmdRob.x || cmdRob.x_cmd != cmdRob.x_cmd){
        std::cout << "maincontrol::checkCmd() ##nan's detected## reverting to IDLE" << std::endl;
        cmdRob.cmdType = "idle";

    }
}

void debugOption(){

}
