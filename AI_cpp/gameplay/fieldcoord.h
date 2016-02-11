#ifndef FIELDCOORD_H
#define FIELDCOORD_H

#include <string>
#include <cstddef>
#include "../dataclasses/gamestatus.h"


class FieldCoord{
public:
    //global variable that is updated by vision
	GameStatus currentStatus;
	GameStatus visionStatus;

	//constructor
	FieldCoord();
    void updateStatus(GameStatus* vision_msg);
};


#endif // FIELDCORD_H
