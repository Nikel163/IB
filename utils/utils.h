#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <ctime>
#include <io.h>
#include <random>

#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

uint32_t* generateKey();
size_t getFileSize(FILE* in);
char* getEncFileName(const char* fileName);
void GNN_teaEncrypt(uint32_t* GNN_block, const uint32_t* GNN_k);
std::string getRestoredFileName(const char* oldFileName);
void setHiddenInput(bool enable);

#endif
