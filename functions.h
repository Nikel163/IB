#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <cstring>
#include "utils.h"

void GNN_startEncrypt(const char* inputFileName, const char* outputFileName, uint32_t* GNN_k);
void GNN_startDecrypt(const char* fileName, const char* keyFileName);

#endif //LAB1_FUNCTIONS_H
