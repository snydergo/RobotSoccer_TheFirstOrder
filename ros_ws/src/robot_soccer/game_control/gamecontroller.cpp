#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <sstream>
#include <iostream>
#include <string>

#include <ros/ros.h>

#include <robot_soccer/gameparam.h>
#include <std_msgs/String.h>


using namespace std;


std::string parseColor() {
    string newValue;
    cin >> newValue;
    if (newValue != ":") {
        cout << "### invalid parameter assingment ###" << endl;
        return "";
    }
    cin >> newValue;
    if (newValue == "green" ||
        newValue == "blue" ||
        newValue == "purple" ||
        newValue == "red" ||
        newValue == "orange" ||
        newValue == "yellow") {
        return newValue;
    } else {
        cout << "### invalid color ###" << endl;
        return "";
    }
}

int main(int argc, char** argv)
{
    std::cout << "RUNNING GAME CONTROLLER" << std::endl;
    ros::init(argc, argv, "game_controller");

    ros::NodeHandle n;

    ros::Publisher gameStatePub = n.advertise<robot_soccer::gameparam>("game_state", 5);
    ros::Publisher gameCommandPub = n.advertise<std_msgs::String>("game_cmd", 5);

    bool paramValid = false;
    bool cmdValid = false;

    std_msgs::String gameCommand;
    robot_soccer::gameparam gameParam;

    gameParam.ally1_color = string("green");
    gameParam.ally2_color = string("purple");
    gameParam.enemy1_color = string("orange");
    gameParam.enemy2_color = string("blue");
    gameParam.field_pos = string("home");
    gameParam.ally_robot_count = 1;
    gameParam.enemy_robot_count = 0;

    while (ros::ok())
    {
        paramValid = false;
        while (!paramValid && !cmdValid) {
            cout << "Input parameter or command:" << endl;
            string parameterName;
            cin >> parameterName;
            if(parameterName == "help"){
                cout << "--- valid (paramter : value) pairs ---" << endl
                    << "ally1_color : green, blue, purple, red, orange, yellow" << endl
                    << "ally2_color : green, blue, purple, red, orange, yellow" << endl
                    << "enemy1_color : green, blue, purple, red, orange, yellow" << endl
                    << "enemy2_color : green, blue, purple, red, orange, yellow" << endl
                    << "field_pos : home, away" << endl
                    << "ally_robot_count : [0-2]" << endl
                    << "enemy_robot_count : [0-2]" << endl << endl
                    << "--- valid commands ---" << endl
                    << "stop (stop robot)" << endl
                    << "start (begin AI state machine)" << endl
                    << "mark (move to starting mark)" << endl
                    << "update (force update of latest paramter values)" << endl << endl
                    << "type 'quit' to quit" << endl << endl;
            } else if (parameterName == "ally1_color") {
                string color = parseColor();
                if (color != "") {
                    gameParam.ally1_color = color;
                    paramValid = true;
                }
            } else if (parameterName == "ally2_color") {
                string color = parseColor();
                if (color != "") {
                    gameParam.ally2_color = color;
                    paramValid = true;
                }
            } else if (parameterName == "enemy1_color") {
                string color = parseColor();
                if (color != "") {
                    gameParam.enemy1_color = color;
                    paramValid = true;
                }
            } else if (parameterName == "enemy2_color") {
                string color = parseColor();
                if (color != "") {
                    gameParam.enemy2_color = color;
                    paramValid = true;
                }
            } else if (parameterName == "field_pos") {
                string pos;
                cin >> pos;
                if (pos != ":") {
                    cout << "### invalid assignment operator ###" << endl;
                    continue;
                }
                cin >> pos;
                if (pos == "home" || pos == "away") {
                    gameParam.field_pos = pos;
                    paramValid = true;
                }
            } else if (parameterName == "ally_robot_count") {
                string pos;
                cin >> pos;
                if (pos != ":") {
                    cout << "### invalid assignment operator ###" << endl;
                    continue;
                }
                cin >> pos;
                int robotCount;
                try{
                    robotCount = stoi(pos);
                }catch(...){
                    std::cout << "### invalid robot count ###\n";
                    continue;
                }
                if (robotCount >= 0 && robotCount <= 2) {
                    gameParam.ally_robot_count = robotCount;
                    paramValid = true;
                } else {
                    std::cout << "### current configuration only supports 0-2 ally robots ###\n";
                    continue;
                }

            } else if (parameterName == "enemy_robot_count") {
                string pos;
                cin >> pos;
                if (pos != ":") {
                    cout << "### invalid assignment operator ###" << endl;
                    continue;
                }
                cin >> pos;
                int robotCount;
                try{
                    robotCount = stoi(pos);
                }catch(...){
                    std::cout << "### invalid robot count ###\n";
                    continue;
                }
                if (robotCount >= 0 && robotCount <= 2) {
                    gameParam.enemy_robot_count = robotCount;
                    paramValid = true;
                } else {
                    std::cout << "### current configuration only supports 0-2 enemy robots ###\n";
                    continue;
                }
            } else if (parameterName == "update") {

                paramValid = true;
            } else if (parameterName == "stop") {
                gameCommand.data = "stop";
                cmdValid = true;
            } else if (parameterName == "start") {
                gameCommand.data = "start";
                cmdValid = true;
            } else if (parameterName == "mark") {
                gameCommand.data = "mark";
                cmdValid = true;
            } else if (parameterName == "quit") {
                return 0;
            } else {
                cout << "### Invalid Parameter or Command ###" << endl;
            }
        }
        if (paramValid) {
            gameStatePub.publish(gameParam);
            paramValid = false;
        }

        if (cmdValid) {
            gameCommandPub.publish(gameCommand);
            cmdValid = false;
        }

    }

}
