#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <cstring>

void GNN_startEncrypt(const std::string& fileName);
void GNN_startDecrypt(const std::string& fileName, const std::string& keyFileName);

#endif //LAB1_FUNCTIONS_H
