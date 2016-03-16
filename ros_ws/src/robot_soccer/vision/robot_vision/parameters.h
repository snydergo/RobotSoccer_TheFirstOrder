
#pragma once

#include "hsvcolorsubspace.h"
#include <string>

void loadConfigData(int argc, char** argv);

HsvColorSubSpace getColorSpace(std::string colorName);
