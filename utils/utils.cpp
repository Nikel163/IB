
#include <random>
#include "utils.h"

using namespace std;

uint32_t* generateKey() {
    mt19937 GNN_gen;
    GNN_gen.seed(time(0));

    auto* GNN_k = new uint32_t[4];

    for (int i = 0; i < 4; i++) {
        char* GNN_buf = new char[4];
        for (int j = 0; j < 4; ++j) {
            GNN_buf[j] = GNN_gen() % 256;
        }
        memcpy(&GNN_k[i], GNN_buf, 4);
    }
    return GNN_k;
}

long getFileSize(FILE* in) {
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    rewind(in);
    return size;
}

void writeKeyToFile(uint32_t* GNN_k) {
    FILE* keyFile = fopen("key.txt", "wb");
    fwrite(GNN_k, 16, 1, keyFile);
    fclose(keyFile);
}

char* getEncFileName(const char* fileName) {
    char* newFileName = strdup(fileName);
    strcat(newFileName, ".enc");
    return newFileName;
}

void GNN_teaEncrypt(uint32_t* GNN_block, const uint32_t* GNN_k) {
    uint32_t GNN_delta = 0x9e3779b9,
             GNN_sum = 0;

    int GNN_n = 32;
    while(GNN_n--) {
        GNN_sum += GNN_delta;
        GNN_block[0] += (( GNN_block[1] << 4 ) + GNN_k[0] ) ^ (GNN_block[1] + GNN_sum ) ^ (( GNN_block[1] >> 5 ) + GNN_k[1] );
        GNN_block[1] += (( GNN_block[0] << 4 ) + GNN_k[2] ) ^ (GNN_block[0] + GNN_sum ) ^ (( GNN_block[0] >> 5 ) + GNN_k[3] );
    }
}

string getRestoredFileName(const char* encryptedFileName) {
    string fileName(encryptedFileName);
    string GNN_newFileName = fileName.substr(0, fileName.size() - 4);

    // append (<number>) if file exists, i.e. file(1).txt
    string GNN_finalFileName = GNN_newFileName;
    int GNN_i = 1;
    while (_access(GNN_finalFileName.c_str(), F_OK) == 0) {
        string GNN_number = string("(") + to_string(GNN_i++) + string(")");
        GNN_finalFileName = GNN_newFileName;
        GNN_finalFileName.insert(GNN_finalFileName.rfind('.'), GNN_number);
    }

    return GNN_finalFileName;
}
