#include "fieldcoord.h"

void FieldCoord::updateStatus(const GameStatus &vision_msg){
    visionStatus = vision_msg;
}
