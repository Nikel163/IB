#ifndef GLOBAL_H
#define GLOBAL_H

#include "../utils/utils.h"
#include "rle.h"
#include "ofb.h"
#include "base64.h"

void makeItBeautiful(char* inputFileName, const std::string& password, bool isArchiving,
                     bool withSessionKey, bool isNeedBase64Encode);
void bringMeBack(const std::string& fileName, const std::string& password, bool withSessionKey);

#endif