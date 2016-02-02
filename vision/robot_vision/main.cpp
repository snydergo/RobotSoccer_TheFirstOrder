#include <stdio.h>
#include "hsvcolorsubspace.h"
#include "utils.h"
#include "objectdetection.h"
#include "config.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/filereadstream.h>

using namespace rapidjson;
using namespace std;
using namespace cv;

void loadConfigData(int argc, char** argv);

int main(int argc, char** argv)
{
    if (argc != 2) { cout << "no param file. Usage: program [param.json]\n"; return 1; }
    loadConfigData(argv);
}

void loadConfigData(char** argv)
{
    FILE* pFile = fopen(argv[1], "r");
    char buffer[65536];
    FileReadStream is(pFile, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);

    Value& root = document["param"];
    
    Value&  calibrate = root.member("calibrate");
    
    fieldCenter_px.x = calibrate.member("fieldCenterX_pixel");
    fieldCenter_px.y = calibrate.member("fieldCenterY_pixel");
    
    cmPerPixelConversionFactor = calibrate.member("referenceDistance_cm") / calibrate.member("referenceDistance_pixel");
    
    invertX = calibrate.member("invertDirectionX");
    
    Value& crop = root.member("crop");
    
    cropLeft = crop.member("left");
    cropRight = crop.member("right");
    cropTop = crop.member("top");
    cropBottom = crop.member("bottom");
    
    Value& ballColorHsv = root.member("ball").member("color");
    
    ballColor.hue.low         = ballColorHsv.member("lowH");
    ballColor.hue.high        = ballColorHsv.member("highH");
    ballColor.saturation.low  = ballColorHsv.member("lowS");
    ballColor.saturation.high = ballColorHsv.member("highS");
    ballColor.value.low       = ballColorHsv.member("lowV");
    ballColor.value.high      = ballColorHsv.member("highV");
    
    teamRobotCount = root.member("teamRobots").member("count");
    
    Value& teamRobotPrimary = root.member("teamRobots").member("primaryColor");
    teamRobotPrimaryColor.hue.low         = teamRobotPrimary.member("lowH");
    teamRobotPrimaryColor.hue.high        = teamRobotPrimary.member("highH");
    teamRobotPrimaryColor.saturation.low  = teamRobotPrimary.member("lowS");
    teamRobotPrimaryColor.saturation.high = teamRobotPrimary.member("highS");
    teamRobotPrimaryColor.value.low       = teamRobotPrimary.member("lowV");
    teamRobotPrimaryColor.value.high      = teamRobotPrimary.member("highV");
    
    Value& teamRobotSecondary = root.member("teamRobots").member("secondaryColor");
    teamRobotSecondaryColor.hue.low         = teamRobotSecondary.member("lowH");
    teamRobotSecondaryColor.hue.high        = teamRobotSecondary.member("highH");
    teamRobotSecondaryColor.saturation.low  = teamRobotSecondary.member("lowS");
    teamRobotSecondaryColor.saturation.high = teamRobotSecondary.member("highS");
    teamRobotSecondaryColor.value.low       = teamRobotSecondary.member("lowV");
    teamRobotSecondaryColor.value.high      = teamRobotSecondary.member("highV");
    
    opponentRobotCount = root.member("opponentRobots").member("count");
    
    Value& opponentRobotPrimary = root.member("opponentRobots").member("primaryColor");
    opponentRobotPrimaryColor.hue.low         = opponentRobotPrimary.member("lowH");
    opponentRobotPrimaryColor.hue.high        = opponentRobotPrimary.member("highH");
    opponentRobotPrimaryColor.saturation.low  = opponentRobotPrimary.member("lowS");
    opponentRobotPrimaryColor.saturation.high = opponentRobotPrimary.member("highS");
    opponentRobotPrimaryColor.value.low       = opponentRobotPrimary.member("lowV");
    opponentRobotPrimaryColor.value.high      = opponentRobotPrimary.member("highV");
    
    Value& opponentRobotSecondary = root.member("opponentRobots").member("secondaryColor");
    opponentRobotSecondaryColor.hue.low         = opponentRobotSecondary.member("lowH");
    opponentRobotSecondaryColor.hue.high        = opponentRobotSecondary.member("highH");
    opponentRobotSecondaryColor.saturation.low  = opponentRobotSecondary.member("lowS");
    opponentRobotSecondaryColor.saturation.high = opponentRobotSecondary.member("highS");
    opponentRobotSecondaryColor.value.low       = opponentRobotSecondary.member("lowV");
    opponentRobotSecondaryColor.value.high      = opponentRobotSecondary.member("highV");
    
    File.close();
}
