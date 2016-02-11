#include "fieldobject.h"

//Field Object Functions
FieldObject::FieldObject(std::string tag, Point loc, Point vel){
    this->tag = tag;
    this->location = loc;
    this->velocity = vel;
}

std::string FieldObject::toString(){
    return "null";
}

