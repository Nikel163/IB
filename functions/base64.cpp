#include <bitset>
#include "base64.h"

using namespace std;

static unsigned char encodingTable[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


void base64encode(const char* inputFileName, const char* outputFileName) {
    checkFileExist(inputFileName);

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(outputFileName, "wb");

    // buffer for input data
    unsigned char GNN_buf[3];
    // buffer for output data
    unsigned char GNN_result[4];

    size_t GNN_fileSize = getFileSize(GNN_input);
    size_t GNN_blockSize = sizeof(GNN_buf);
    size_t GNN_resultBlockSize = sizeof(GNN_result);
    size_t GNN_blocksNumber = GNN_fileSize / GNN_blockSize;
    size_t GNN_tailSize = GNN_fileSize % GNN_blockSize;

    while (GNN_blocksNumber--) {
        fread(GNN_buf, GNN_blockSize, 1, GNN_input);

        GNN_result[0] = encodingTable[GNN_buf[0] >> 2];
        GNN_result[1] = encodingTable[((GNN_buf[0] & 3) << 4) | (GNN_buf[1] >> 4)];
        GNN_result[2] = encodingTable[(GNN_buf[1] & 15) << 2 | (GNN_buf[2] >> 6)];
        GNN_result[3] = encodingTable[GNN_buf[2] & 63];

        fwrite(GNN_result, GNN_resultBlockSize, 1, GNN_output);
    }

    // encoding block with size not equal 3
    if (GNN_tailSize > 0) {
        fread(GNN_buf, GNN_tailSize, 1, GNN_input);

        GNN_result[0] = encodingTable[GNN_buf[0] >> 2];
        GNN_result[1] = encodingTable[((GNN_buf[0] & 3) << 4) | (GNN_buf[1] >> 4)];
        GNN_result[2] = GNN_tailSize == 2 ? encodingTable[(GNN_buf[1] & 15) << 2] : '=';
        GNN_result[3] = '=';

        fwrite(GNN_result, GNN_resultBlockSize, 1, GNN_output);
    }

    fclose(GNN_input);
    fclose(GNN_output);

    cout << "File " << outputFileName << " created\n";
    cout << "Base64 encoding successfully completed\n\n";
}

string base64decode(const char* inputFileName, const char* outputFileName) {
    checkFileExist(inputFileName);

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(outputFileName, "wb");

    // buffer for input data in base64 format
    unsigned char GNN_base64block[4];
    // buffer for data in ASCII format
    unsigned char GNN_buf[4];
    // buffer for output data
    unsigned char GNN_result[3];

    size_t GNN_fileSize = getFileSize(GNN_input);
    size_t GNN_blockSize = sizeof(GNN_base64block);
    size_t GNN_blocksNumber = GNN_fileSize / GNN_blockSize;

    if (GNN_fileSize % 4 != 0) {
        cerr << "Decoding error: file size not divisible by 4\n";
        abort();
    }

    // generate decoding table
    unsigned char GNN_decodingTable[256];
    for (int i = 0; i < 64; i++) {
        GNN_decodingTable[encodingTable[i]] = i;
    }

    while (GNN_blocksNumber--) {
        fread(GNN_base64block, GNN_blockSize, 1, GNN_input);

        // get size for output data
        int GNN_resultBlockSize;
        if (GNN_base64block[3] == '=' && GNN_base64block[2] == '=') {
            GNN_resultBlockSize = 1;
        } else if (GNN_base64block[3] == '=') {
            GNN_resultBlockSize = 2;
        } else {
            GNN_resultBlockSize = 3;
        }

        // convert base64 to ASCII
        for (int i = 0; i < 4; i++) {
            GNN_buf[i] = GNN_decodingTable[GNN_base64block[i]];
        }

        GNN_result[0] = (GNN_buf[0] << 2) | (GNN_buf[1] >> 4);
        GNN_result[1] = (GNN_buf[1] << 4) | (GNN_buf[2] >> 2);
        GNN_result[2] = (GNN_buf[2] << 6) | GNN_buf[3];

        fwrite(GNN_result, GNN_resultBlockSize, 1, GNN_output);
    }

    cout << "File " << outputFileName << " created\n";
    cout << "Base64 decoding successfully completed\n\n";

    fclose(GNN_input);
    fclose(GNN_output);

    return outputFileName;
}
