#ifndef FIELDCOORD_H
#define FIELDCOORD_H

#include <string>
#include <cstddef>
#include "../dataclasses/GameStatus.h"


class FieldCoord{
public:
    //global variable that is updated by vision
	GameStatus currentStatus;
	GameStatus visionStatus;

	//constructor
	FieldCoord();
};


#endif // FIELDCORD_H