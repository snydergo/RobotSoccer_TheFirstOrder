#include "bookkeeping.h"

bool pend(sem semaphore) {
    if(semaphore){
        semaphore--;
        return true;
    }else
        return false;
}

void post(sem semaphore) {
    semaphore++;
}

Point center;
FieldCoord field;
bool visionUpdated;
bool sendMessage;
sem visionMsg_sem = 1;
sem ctrlMsg_sem = 1;
GameStatus visionStatus_msg;

