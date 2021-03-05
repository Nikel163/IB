#include "ofb.h"
#include "../utils/md5.hpp"

using namespace std;

void GNN_startOFB(const char* inputFileName, const char* outputFileName, uint32_t* GNN_sessionKey, string password, boolean isEncrypting) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    FILE* GNN_in = fopen(inputFileName, "rb");
    FILE* GNN_out = fopen(outputFileName, "wb");

    uint32_t GNN_initializationVector[2] = {0xB000BAAA, 0x14133788};
    uint32_t GNN_block[2];
    uint32_t GNN_blockEncrypted[2];

    uint32_t GNN_blockSize = sizeof(GNN_block);

    long GNN_fileSize = isEncrypting ? getFileSize(GNN_in) : getFileSize(GNN_in) - 16;
    uint32_t GNN_tailSize = GNN_fileSize % GNN_blockSize;
    uint32_t GNN_blocksNumber = GNN_fileSize / GNN_blockSize;

    cout << "File size: " << GNN_fileSize << " bytes" << endl;
    cout << "Last block size: " << GNN_tailSize << " bytes" << endl;
    cout << "Blocks number: " << GNN_blocksNumber << endl;

    auto hash = md5(password);
    for (int i = 0; i <= 2; i+=2) {
        uint32_t block[2] = {GNN_sessionKey[i], GNN_sessionKey[i + 1]};

        GNN_teaEncrypt(GNN_initializationVector, hash);

        GNN_blockEncrypted[0] = block[0] ^ GNN_initializationVector[0];
        GNN_blockEncrypted[1] = block[1] ^ GNN_initializationVector[1];

        if (isEncrypting) {
            fwrite(GNN_blockEncrypted, 8, 1, GNN_out);
        } else {
            GNN_sessionKey[i] = GNN_blockEncrypted[0];
            GNN_sessionKey[i+1] = GNN_blockEncrypted[1];
        }
    }

    // skip session key if decrypting file
    if (!isEncrypting) {
        fseek(GNN_in, 16, SEEK_SET);
    }

    while (GNN_blocksNumber--) {
        // read block for encrypt
        fread(GNN_block, GNN_blockSize, 1, GNN_in);

        // ------------- start of encrypting -------------------
        GNN_teaEncrypt(GNN_initializationVector, GNN_sessionKey);
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }
        // ------------- end of encrypting -------------------
        fwrite(GNN_blockEncrypted, GNN_blockSize, 1, GNN_out);
    }

    if (GNN_tailSize > 0) {
        // read last block which not divisible by 8
        fread(GNN_block, GNN_tailSize, 1, GNN_in);

        GNN_teaEncrypt(GNN_initializationVector, GNN_sessionKey);
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }

        fwrite(GNN_blockEncrypted, GNN_tailSize, 1, GNN_out);
    }
    fclose(GNN_in);
    fclose(GNN_out);
}