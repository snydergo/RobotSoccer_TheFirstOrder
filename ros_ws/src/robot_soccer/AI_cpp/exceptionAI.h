#ifndef EXCEPTIONAI_H
#define EXCEPTIONAI_H

#include <string>
#include <cstddef>

enum class errtype {parsedata, senddata, robotType, outofbounds, none};

class ExceptionAI
{
public:
    errtype err;

    //Exceptions functions
    ExceptionAI(errtype err);
    std::string toString();
};

#endif // EXCEPTIONAI_H
