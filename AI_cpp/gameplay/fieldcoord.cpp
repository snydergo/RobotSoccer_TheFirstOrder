#include "fieldcoord.h"

FieldCoord::FieldCoord(){
	
}

void FieldCoord::updateStatus(GameStatus* vision_msg){
    this->visionStatus = *vision_msg;
}
