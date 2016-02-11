#include <cstdio>

#include "bookkeeping.h"


int main(int argc, char *argv[])
{
    GameStatus* game = initGameStatus();
    field.currentStatus = *game;
    field.visionStatus = *game;
    while(1){
        if(visionUpdated){
            visionUpdated = false;
            field.updateStatus(&visionStatus_msg);
        }
    }
    return 0;
}







