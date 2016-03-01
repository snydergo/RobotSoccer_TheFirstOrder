#include "parameters.h"

#include "hsvcolorsubspace.h"
#include "config.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace rapidjson;

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

}
