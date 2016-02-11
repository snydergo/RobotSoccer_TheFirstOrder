#ifndef EXCEPTIONAI_H
#define EXCEPTIONAI_H

#include <string>
#include <cstddef>

enum errtype {parsedata, senddata, none};

class ExceptionAI
{
public:
    errtype err = none;

    //Exceptions functions
    ExceptionAI(errtype err);
    std::string toString();
};

#endif // EXCEPTIONAI_H
