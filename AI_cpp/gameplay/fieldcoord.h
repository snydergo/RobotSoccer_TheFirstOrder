#ifndef FIELDCOORD_H
#define FIELDCOORD_H

#include <string>

#include "../dataclasses/gamestatus.h"


class FieldCoord{
public:
    //global variable that is updated by vision
	GameStatus currentStatus;
	GameStatus visionStatus;

    void updateStatus(const GameStatus &vision_msg);
};


#endif // FIELDCORD_H
