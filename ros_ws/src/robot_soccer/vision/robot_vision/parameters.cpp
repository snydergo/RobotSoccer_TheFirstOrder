#include "parameters.h"

#include "hsvcolorsubspace.h"
#include "config.h"

//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
//#include "rapidjson/filereadstream.h"

#include <iostream>
#include <stdio.h>

using namespace std;

HsvColorSubSpace getColorSpace(string colorName)
{
    if (colorName == "green") return config::green;
    if (colorName == "blue") return config::blue;
    if (colorName == "purple") return config::purple;
    if (colorName == "red") return config::red;
    if (colorName == "orange") return config::orange;
    if (colorName == "pink") return config::pink;
    if (colorName == "yellow") return config::yellow;
    return config::green;
}

void loadConfigData(int argc, char** argv)
{
    config::ballArea = 50;
    config::ballColor = config::pink;
    config::allyRobot1Color = config::green;
    config::allyRobot2Color = config::purple;
    config::enemyRobot1Color = config::orange;
    config::enemyRobot2Color = config::blue;
    config::allyRobotCount = 1;
    config::enemyRobotCount = 0;
    config::cropTop = 0;
    config::cropBottom = 0;
    config::cropLeft = 0;
    config::cropRight = 0;
    config::trackRobot = false;
    config::showVideo = true;
    if (argc == 2) {
        try {
            int ip;
            ip = stoi(argv[1]);
            if (ip == 78) {
                cout << "loading camera 192.168.1.78..." << endl;
                config::cameraUrl = "http://192.168.1.78:8080/stream?topic=/image&dummy=param.mjpg";
                config::fieldCenter_px = Point2f(449, 224);
                config::cmPerPixelConversionFactor = 154.2 / 276.0;
                config::invertX = true; // we are home
                config::homeIsInverted = true;
                return;
            }
        } catch(...) {}
    }
    // default
    cout << "loading camera 192.168.1.79..." << endl;
    config::cameraUrl = "http://192.168.1.79:8080/stream?topic=/image&dummy=param.mjpg";
    config::fieldCenter_px = Point2f(413, 222);
    config::cmPerPixelConversionFactor = 100.0 / 179.0;
    config::invertX = false; // we are home
    config::homeIsInverted = false;
    return;
}

/*
    cout << "loading param file..." << endl;
    FILE* file;
    if (argc == 1) {
        file = fopen("src/robot_soccer/vision/robot_vision/param.json", "r");
    } else {
        file = fopen(argv[1], "r");
    }

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
    config::trackRobot = calibrate["trackRobot"].GetBool();

    Value& crop = root["crop"];

    config::cropLeft = crop["left"].GetInt();
    config::cropRight = crop["right"].GetInt();
    config::cropTop = crop["top"].GetInt();
    config::cropBottom = crop["bottom"].GetInt();

    Value& ball = root["ball"];
    config::ballArea = ball["area"].GetInt();
    config::ballColor = getColorSpace(ball["color"].GetString());

    Value& teamRobot = root["teamRobots"];
    config::teamRobotCount = teamRobot["count"].GetInt();
    config::teamRobotLargeArea = teamRobot["large_area"].GetInt();
    config::teamRobotSmallArea = teamRobot["small_area"].GetInt();
    config::teamRobotPrimaryColor = getColorSpace(teamRobot["color"].GetString());

    Value& opponentRobot = root["opponentRobots"];
    config::opponentRobotCount = opponentRobot["count"].GetInt();
    config::opponentRobotLargeArea = opponentRobot["large_area"].GetInt();
    config::opponentRobotSmallArea = opponentRobot["small_area"].GetInt();
    config::opponentRobotPrimaryColor = getColorSpace(opponentRobot["color"].GetString());

    fclose(file);
    cout << "param file successfully loaded" << endl;
*/
