#include "fieldcoord.h"

void FieldCoord::updateStatus(const GameStatus &vision_msg){
    visionStatus = vision_msg;
    //first time we don't want to compute velocity
    if(this->firstTime){
        this->firstTime = false;
    }else
        this->updateVelocities();
    //update currentstatus
    /*std::cout << "ally1 x == " + std::to_string(visionStatus.ally1.location.x) << std::endl <<
                 "ally1 y == " + std::to_string(visionStatus.ally1.location.y) << std::endl <<
                 "ally1 theta == " + std::to_string(visionStatus.ally1.theta) << std::endl;
    */
    currentStatus = visionStatus;
}

void FieldCoord::updateVelocities(){
    //std::cout << "updating the velocities" << std::endl;
    visionStatus.ally1.velocity = calc::getVelocity(visionStatus.ally1, currentStatus.ally1);
    visionStatus.ally2.velocity = calc::getVelocity(visionStatus.ally2, currentStatus.ally2);
    visionStatus.enemy1.velocity = calc::getVelocity(visionStatus.enemy1, currentStatus.enemy1);
    visionStatus.enemy2.velocity = calc::getVelocity(visionStatus.enemy2, currentStatus.enemy2);
    visionStatus.ball.velocity = calc::getVelocity(visionStatus.ball, currentStatus.ball);
}

