#include <io.h>
#include "functions.h"

using namespace std;

void GNN_startEncrypt(const char* inputFileName, const char* outputFileName, uint32_t* GNN_k) {
    FILE* GNN_in = fopen(inputFileName, "rb");

    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    FILE* GNN_out = fopen(outputFileName, "wb");

    uint32_t GNN_initializationVector[2] = {0xBABAFFFF, 0xFFFFAAAA};
    uint32_t GNN_block[2];
    uint32_t GNN_blockEncrypted[2];

    uint32_t GNN_blockSize = sizeof(GNN_block);

    long GNN_fileSize = getFileSize(GNN_in);
    uint32_t GNN_tailSize = GNN_fileSize % GNN_blockSize;
    uint32_t GNN_blocksNumber = GNN_fileSize / GNN_blockSize;

    cout << "File size: " << GNN_fileSize << " bytes" << endl;
    cout << "Last block size: " << GNN_tailSize << " bytes" << endl;
    cout << "Blocks number: " << GNN_blocksNumber << endl;

    while (GNN_blocksNumber--) {
        // read block for encrypt
        fread(GNN_block, GNN_blockSize, 1, GNN_in);

        // ------------- start of encrypting -------------------
        GNN_teaEncrypt(GNN_initializationVector, GNN_k);
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }
        // ------------- end of encrypting -------------------
        fwrite(GNN_blockEncrypted, GNN_blockSize, 1, GNN_out);
    }

    if (GNN_tailSize > 0) {
        // read last block which not divisible by 8
        fread(GNN_block, GNN_tailSize, 1, GNN_in);

        GNN_teaEncrypt(GNN_initializationVector, GNN_k);
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }

        fwrite(GNN_blockEncrypted, GNN_tailSize, 1, GNN_out);
    }
}

void GNN_startDecrypt(const char* encryptedFileName, const char* keyFileName) {
    // check file with key
    if (_access(keyFileName, F_OK) != 0) {
        cerr << "File " << keyFileName << " not found\n";
        abort();
    }

    // reading key from file
    uint32_t GNN_k[4];
    FILE* keyFile = fopen(keyFileName, "rb");
    fread(GNN_k, 16, 1, keyFile);

    // start decrypting
    GNN_startEncrypt(encryptedFileName, getRestoredFileName(encryptedFileName).c_str(), GNN_k);
}