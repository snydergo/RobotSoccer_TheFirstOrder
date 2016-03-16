#ifndef FIELDCOORD_H
#define FIELDCOORD_H

#include <string>
#include "../mathfunctions.h"
#include "../dataclasses/gamestatus.h"


class FieldCoord{
public:
    //global variable that is updated by vision
	GameStatus currentStatus;
	GameStatus visionStatus;
    bool firstTime;
    FieldCoord()
        : currentStatus(GameStatus())
        , visionStatus(GameStatus())
        , firstTime(false)
    {
    }

    void updateStatus(const GameStatus &vision_msg);

    void updateVelocities();
};


#endif // FIELDCORD_H
