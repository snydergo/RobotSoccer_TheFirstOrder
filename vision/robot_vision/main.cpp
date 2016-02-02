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

void loadConfigData(char** argv);
vector<cv::Moments> locateCvObjects(const cv::Mat& frame, const HsvColorSubSpace& colorSegment);

int main(int argc, char** argv)
{
    if (argc != 2) { cout << "no param file. Usage: program [param.json]\n"; return 1; }
    loadConfigData(argv);
    
    cv::VideoCapture camera = ConnectCamera(config::cameraUrl);
    
    while (true) {
        cv::Mat frame = ReadFrame(camera);
        
        /// find our robots
        vector<cv::Moments> teamMoments = locateCvObjects(frame, config::teamRobotPrimaryColor);
        // find their robots
        vector<cv::Moments> opponetMoments = locateCvObjects(frame, config::opponentRobotPrimaryColor);
        //find the ball
        vector<cv::Moments> balls = locateCvObjects(frame, config::ballColor);
        
        
    }
    
    return 0;
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

    config::fieldCenter_px.x = calibrate["fieldCenterX_pixel"].GetInt();
    config::fieldCenter_px.y = calibrate["fieldCenterY_pixel"].GetInt();

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
    
    Value& teamRobotPrimary = root["teamRobots"]["primaryColor"];
    config::teamRobotPrimaryColor.hue.low         = teamRobotPrimary["lowH"].GetInt();
    config::teamRobotPrimaryColor.hue.high        = teamRobotPrimary["highH"].GetInt();
    config::teamRobotPrimaryColor.saturation.low  = teamRobotPrimary["lowS"].GetInt();
    config::teamRobotPrimaryColor.saturation.high = teamRobotPrimary["highS"].GetInt();
    config::teamRobotPrimaryColor.value.low       = teamRobotPrimary["lowV"].GetInt();
    config::teamRobotPrimaryColor.value.high      = teamRobotPrimary["highV"].GetInt();
    
    Value& teamRobotSecondary = root["teamRobots"]["secondaryColor"];
    config::teamRobotSecondaryColor.hue.low         = teamRobotSecondary["lowH"].GetInt();
    config::teamRobotSecondaryColor.hue.high        = teamRobotSecondary["highH"].GetInt();
    config::teamRobotSecondaryColor.saturation.low  = teamRobotSecondary["lowS"].GetInt();
    config::teamRobotSecondaryColor.saturation.high = teamRobotSecondary["highS"].GetInt();
    config::teamRobotSecondaryColor.value.low       = teamRobotSecondary["lowV"].GetInt();
    config::teamRobotSecondaryColor.value.high      = teamRobotSecondary["highV"].GetInt();

    config::opponentRobotCount = root["opponentRobots"]["count"].GetInt();
    
    Value& opponentRobotPrimary = root["opponentRobots"]["primaryColor"];
    config::opponentRobotPrimaryColor.hue.low         = opponentRobotPrimary["lowH"].GetInt();
    config::opponentRobotPrimaryColor.hue.high        = opponentRobotPrimary["highH"].GetInt();
    config::opponentRobotPrimaryColor.saturation.low  = opponentRobotPrimary["lowS"].GetInt();
    config::opponentRobotPrimaryColor.saturation.high = opponentRobotPrimary["highS"].GetInt();
    config::opponentRobotPrimaryColor.value.low       = opponentRobotPrimary["lowV"].GetInt();
    config::opponentRobotPrimaryColor.value.high      = opponentRobotPrimary["highV"].GetInt();

    Value& opponentRobotSecondary = root["opponentRobots"]["secondaryColor"];
    config::opponentRobotSecondaryColor.hue.low         = opponentRobotSecondary["lowH"].GetInt();
    config::opponentRobotSecondaryColor.hue.high        = opponentRobotSecondary["highH"].GetInt();
    config::opponentRobotSecondaryColor.saturation.low  = opponentRobotSecondary["lowS"].GetInt();
    config::opponentRobotSecondaryColor.saturation.high = opponentRobotSecondary["highS"].GetInt();
    config::opponentRobotSecondaryColor.value.low       = opponentRobotSecondary["lowV"].GetInt();
    config::opponentRobotSecondaryColor.value.high      = opponentRobotSecondary["highV"].GetInt();
    fclose(file);

}
