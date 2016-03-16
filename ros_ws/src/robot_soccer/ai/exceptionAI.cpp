#include "exceptionAI.h"

ExceptionAI::ExceptionAI(errtype err){
    this->err = err;
}

std::string ExceptionAI::toString(){
    switch(err){
        case errtype::parsedata:
            break;
        case errtype::senddata:
            break;
        case errtype::outofbounds:
            return "Point is out of Bounds\n";
            break;
        default:
            break;
    }
    return "";
}
