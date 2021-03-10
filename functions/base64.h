#ifndef BASE64_H
#define BASE64_H

#include "../utils/utils.h"

void base64encode(const char* inputFileName, const char* outputFileName);
std::string base64decode(const char* inputFileName, const char* outputFileName);

#endif