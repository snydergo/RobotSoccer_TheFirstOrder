#include "getFilteredData.h"

void filterCallback(const robot_soccer::visiondata& msg){
    predicted = msg;
    predictedUpdated = true;
}
