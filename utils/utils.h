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
void checkFileExist(const char* inputFileName);
uint32_t* read16BytesFromFile(const char* inputFileName);
char* getEncFileName(const char* fileName);
char* getBase64FileName(const char* fileName);
std::string getRestoredFileName(const char* oldFileName);
std::string cutLastExtension(const std::string& fileName);
void GNN_teaEncrypt(uint32_t* GNN_block, const uint32_t* GNN_k);
void setHiddenInput(bool enable);
void printHelp();
void ask(const char* question);
bool parseAnswer();

#endif
