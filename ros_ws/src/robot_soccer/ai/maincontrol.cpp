#include <cstdio>
#include <iostream>
#include <string>
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

void checkCmd(robot_soccer::controldata &cmdRob);
void debugOption();
void checkGCFlags(Strategies &strategies);
void mainControlSM(ros::NodeHandle &n)
{
    stream::info.open("info.txt");
    //PUBLISHER FOR MOTIONCONTROL
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 5);
    ros::Publisher robo2Com = n.advertise<robot_soccer::controldata>("robot2Com", 5);

    //SUBSCRIBER FROM VISION
    ros::Subscriber vision_subscriber = n.subscribe("vision_data", 5, visionCallback);
    (void*)vision_subscriber;

    ros::Rate loop_rate(TICKS_PER_SEC);
    ros::Subscriber gameCmdSub = n.subscribe("game_cmd", 5, gameCmdCallback);
    (void*)gameCmdSub;
    Strategies strategies;
    while (ros::ok()) {
        checkGCFlags(strategies);

        if (visionUpdated) {
            visionUpdated = false;
            field.updateStatus(visionStatus_msg);
            strategies.tick();
        }
        if (sendCmd_Rob1) {
            sendCmd_Rob1 = false;
            checkCmd(cmdRob1);
            robo1Com.publish(cmdRob1);
        }
        if (sendCmd_Rob2) {
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
    ros::Publisher roboCom = n.advertise<robot_soccer::controldata>("robot2Com", 5);

    //SUBSCRIBER FROM VISION
    ros::Subscriber vision_subscriber = n.subscribe("vision_data", 5, visionCallback);
    (void*)vision_subscriber;

    ros::Rate loop_rate(TICKS_PER_SEC);
    bool dataInitialized = false;
    bool init = true;

    ros::Subscriber debug_subscriber = n.subscribe("debug", 5, debugCallback);
    (void*)debug_subscriber;
    Skills skill(RobotType::ally2);
    Point dest;
    bool kickball = false;
    char kickCounter = 0;
    bool kickCmd = false;
    while(ros::ok()) {

        if (visionUpdated) {
            visionUpdated = false;
            dataInitialized = true;
            field.updateStatus(visionStatus_msg);
        }
        if (newDebugCmd || init) {
            init = false;
            newDebugCmd = false;
            cmdRob2.cmdType = debugCmd.cmdType;
            cmdRob2.x_cmd = debugCmd.x_cmd;
            cmdRob2.y_cmd = debugCmd.y_cmd;
            cmdRob2.theta_cmd = debugCmd.theta_cmd;
            if (cmdRob2.cmdType == "moveslow" || cmdRob2.cmdType == "movefast") {
                moveSpeed speed;
                if(cmdRob2.cmdType == "moveslow") {
                    speed = moveSpeed::slow;
                } else
                    speed = moveSpeed::fast;

                dest = Point(cmdRob2.x_cmd,cmdRob2.y_cmd);
                skill.goToPoint(speed, dest, cmdRob2.theta_cmd);
            } else if (cmdRob2.cmdType == "kick") {
                kickball = true;
                skill.kick();
            } else if (cmdRob2.cmdType == "kickinit") {
                skill.init_kick();
                std::cout << "initing kicker" << std::endl;
            } else if (cmdRob2.cmdType == "kickuninit") {
                std::cout << "unit kicker" << std::endl;
                skill.uninit_kick();
            } else {
                skill.idle();
            }
        }

        skill.tick();
        if (sendCmd_Rob2) {
            sendCmd_Rob2 = false;
            checkCmd(cmdRob2);
            roboCom.publish(cmdRob2);
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
    while (ros::ok()) {
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

    ros::Subscriber gameCmdSub = n.subscribe("game_cmd", 5, gameCmdCallback);
    (void*)gameCmdSub;
    Plays play(RobotType::ally2);
    play.playGoalie();
    while (ros::ok()) {
        checkGCFlags(strategies);

        if (predictedUpdated) {
            predictedUpdated = false;
            dataInitialized = true;
            field.updateStatus(predicted);
        }
        play.tick();

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
    stream::info.open("kickpoint.txt");
    std::string option;
    try {
        option = std::string(argv[OPTION]);
    } catch(...) {
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
    stream::info.close();
    return 0;
}

//function used by GameControl to set flags for change of strategy.
void gameCmdCallback(const std_msgs::String &msg)
{
    if (msg.data == "stop") {
        /// ----------------- send stop command to robot -------------------
        gameControl_flags = STOP;
        std::cout << "Setting Stop Flag\n";
    } else if (msg.data == "start") {
        /// ----------------- start the AI state machine -------------------
        std::cout << "Setting Start Flag\n";
        gameControl_flags = START;
    } else if (msg.data == "mark") {
        /// ----------------- go to starting postion for match -------------
        gameControl_flags = MARK;
        std::cout << "Setting Mark Flag\n";
    } else {
        std::cout << "ERROR: Unknown game command: " << msg << std::endl;
    }
}

//function that checks flags set by GameControl and changes strategies.
void checkGCFlags(Strategies& strategies)
{
    if (gameControl_flags) {
        if (gameControl_flags == START) {
            gameControl_flags = 0;
            std::cout << "responding To START flag\n";
            strategies.start();
        } else if (gameControl_flags == STOP) {
            gameControl_flags = 0;
            std::cout << "responding To STOP flag\n";
            strategies.stop();
        } else if (gameControl_flags == MARK) {
            gameControl_flags = 0;
            std::cout << "responding To MARK flag\n";
            strategies.mark();
        }
    }
}

void checkCmd(robot_soccer::controldata &cmdRob)
{
    if (cmdRob.x != cmdRob.x || cmdRob.x_cmd != cmdRob.x_cmd) {
        std::cout << "maincontrol::checkCmd() ##nan's detected## reverting to IDLE" << std::endl;
        cmdRob.cmdType = "idle";
    } else if (abs(cmdRob.x_cmd) > FIELD_XBORDER) {
        cmdRob.x_cmd = FIELD_XBORDER;
    }
}

void debugOption()
{

}
