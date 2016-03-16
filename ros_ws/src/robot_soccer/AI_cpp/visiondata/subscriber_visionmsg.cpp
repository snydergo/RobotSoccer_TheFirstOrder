#include "subscriber_visionmsg.h"

std::ofstream smoothie0;
std::ofstream smoothie1;
std::ofstream smoothie2;
std::ofstream smoothie3;
std::ofstream raw;

int sampleCount = 0;

double smooth(double &smooth, double raw, double beta){
    smooth -= (beta * (smooth - raw));
    return smooth;
}

void filter(GameStatus &rawData){
    static GameStatus SmoothData0;
    static GameStatus SmoothData1;
    static GameStatus SmoothData2;
    static GameStatus SmoothData3;
    if (sampleCount <= SAMPLENUM && DOSAMPLE > .5) {
        raw << rawData.ally1.location.x << " , ";
        raw << rawData.ally1.location.y << " , ";
        raw << rawData.ally1.theta << " , ";
        smoothie0 << smooth( SmoothData0.ally1.location.x, rawData.ally1.location.x ,BETASMOOTH0)<< " , ";
        smoothie0 << smooth( SmoothData0.ally1.location.y , rawData.ally1.location.y ,BETASMOOTH0)<< " , ";
        smoothie0 << smooth( SmoothData0.ally1.theta , rawData.ally1.theta ,BETASMOOTH0)<< " , ";
        smoothie1 << smooth( SmoothData1.ally1.location.x, rawData.ally1.location.x ,BETASMOOTH1)<< " , ";
        smoothie1 << smooth( SmoothData1.ally1.location.y , rawData.ally1.location.y ,BETASMOOTH1)<< " , ";
        smoothie1 << smooth( SmoothData1.ally1.theta , rawData.ally1.theta ,BETASMOOTH1)<< " , ";
        smoothie2 << smooth( SmoothData2.ally1.location.x, rawData.ally1.location.x ,BETASMOOTH2)<< " , ";
        smoothie2 << smooth( SmoothData2.ally1.location.y , rawData.ally1.location.y ,BETASMOOTH2)<< " , ";
        smoothie2 << smooth( SmoothData2.ally1.theta , rawData.ally1.theta ,BETASMOOTH2)<< " , ";
        smoothie3 << smooth( SmoothData3.ally1.location.x, rawData.ally1.location.x ,BETASMOOTH3)<< " , ";
        smoothie3 << smooth( SmoothData3.ally1.location.y , rawData.ally1.location.y ,BETASMOOTH3)<< " , ";
        smoothie3 << smooth( SmoothData3.ally1.theta , rawData.ally1.theta ,BETASMOOTH3)<< " , ";
    }else{
        smooth( SmoothData0.ally1.location.x, rawData.ally1.location.x,BETASMOOTH0);
        smooth( SmoothData0.ally1.location.y , rawData.ally1.location.y,BETASMOOTH0);
        smooth( SmoothData0.ally1.theta , rawData.ally1.theta,BETASMOOTH0);
        smooth( SmoothData0.ally2.location.x , rawData.ally2.location.x,BETASMOOTH0);
        smooth( SmoothData0.ally2.location.y , rawData.ally2.location.y,BETASMOOTH0);
        smooth( SmoothData0.ally2.theta , rawData.ally2.theta,BETASMOOTH0);
        smooth( SmoothData0.enemy1.location.x , rawData.enemy1.location.x,BETASMOOTH0);
        smooth( SmoothData0.enemy1.location.y , rawData.enemy1.location.y,BETASMOOTH0);
        smooth( SmoothData0.enemy1.theta , rawData.enemy1.theta,BETASMOOTH0);
        smooth( SmoothData0.enemy2.location.x , rawData.enemy2.location.x,BETASMOOTH0);
        smooth( SmoothData0.enemy2.location.y , rawData.enemy2.location.y,BETASMOOTH0);
        smooth( SmoothData0.enemy2.theta , rawData.enemy2.theta,BETASMOOTH0);
        smooth( SmoothData0.ball.location.x , rawData.ball.location.x,BETASMOOTH0);
        smooth( SmoothData0.ball.location.y , rawData.ball.location.y,BETASMOOTH0);
    }
}

void setupFilterStreams(){
    if(!sampleCount++ && DOSAMPLE > .5){
        std::string fileName = "filter"+std::to_string(BETASMOOTH0)+".txt";
        smoothie0.open(fileName);
        smoothie0 << "(";
        fileName = "filter"+std::to_string(BETASMOOTH1)+".txt";
        smoothie1.open(fileName);
        smoothie1 << "(";
        fileName = "filter"+std::to_string(BETASMOOTH2)+".txt";
        smoothie2.open(fileName);
        smoothie2 << "(";
        fileName = "filter"+std::to_string(BETASMOOTH3)+".txt";
        smoothie3.open(fileName);
        smoothie3 << "(";
        fileName = "raw.txt";
        raw.open(fileName);
        raw << "(";
    }
}

void closeFilterStreams(){
    if(sampleCount == SAMPLENUM && DOSAMPLE > .5){
        smoothie0 << ")";
        smoothie1 << ")";
        smoothie2 << ")";
        smoothie3 << ")";
        raw << ")";
        smoothie0.close();
        smoothie1.close();
        smoothie2.close();
        smoothie3.close();
        raw.close();
        std::cout << "\n\n####FINISHED####\n\n" << std::endl;
    }
}

void visionCallback(const robot_soccer::visiondata& msg){
    visionStatus_msg = msg;
    vision_msg = msg;
    visionUpdated = true;
}
