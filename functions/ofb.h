#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../utils/utils.h"

std::string GNN_startOFB(const char* inputFileName, const char* outputFileName, const std::string& password, bool isEncrypting, bool withSessionKey);

#endif