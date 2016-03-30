#ifndef EXCEPTIONAI_H
#define EXCEPTIONAI_H
/* ---------------------------------------------------------------------------
** exceptionai.h
** Contains exceptions that can be thrown when robot is out of boundaries and etc
**
** Author: Glade Snyder
** -------------------------------------------------------------------------*/
#include <string>
#include <cstddef>

enum class errtype {parsedata, senddata, RobotType, outofbounds, none};

class ExceptionAI
{
public:
    errtype err;

    //Exceptions functions
    ExceptionAI(errtype err);
    std::string toString();
};

#endif // EXCEPTIONAI_H
