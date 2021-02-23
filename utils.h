#ifndef UTILS_H
#define UTILS_H

uint32_t* generateKey();
long getFileSize(FILE* in);
void writeKeyToFile(uint32_t* GNN_k);
char* getEncFileName(const char* fileName);
void GNN_teaEncrypt(uint32_t* GNN_block, const uint32_t* GNN_k);
std::string getRestoredFileName(const char* encryptedFileName);

#endif //UTILS_H
