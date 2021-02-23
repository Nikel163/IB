#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../utils/utils.h"

void GNN_startEncrypt(const char* inputFileName, const char* outputFileName, uint32_t* GNN_k);
void GNN_startDecrypt(const char* fileName, const char* keyFileName);

#endif