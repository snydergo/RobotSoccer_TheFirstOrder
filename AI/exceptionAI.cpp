#include "exceptionAI.h"

ExceptionAI::ExceptionAI(errtype err){
    this->err = err;
}

std::string ExceptionAI::toString(){
    std::string str = "";
    switch(err){
        case parsedata:
            break;
        case senddata:
            break;
        default:
            break;
    }
    return str;
}
