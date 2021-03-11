#include "rle.h"

using namespace std;

string compressFile(const char* inputFileName) {
    checkFileExist(inputFileName);

    string GNN_outputFileName(inputFileName);
    GNN_outputFileName.append(".arh");

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(GNN_outputFileName.c_str(), "wb");

    signed char GNN_c = getc(GNN_input);

    // in case empty file
    if (feof(GNN_input)) {
        fputc(GNN_c, GNN_output);
        fclose(GNN_input);
        fclose(GNN_output);
        return GNN_outputFileName;
    }

    // buffer for chain
    signed char GNN_chain[128];
    int GNN_currentChainSize = 0;

    // remember first symbol
    GNN_chain[GNN_currentChainSize] = GNN_c;

    // flag for different logic
    bool repeated = false;

    while (!feof(GNN_input)) {
        // if reading same symbols
        if (repeated) {
            // if get another symbol
            if (GNN_currentChainSize >= 2 && GNN_c != GNN_chain[GNN_currentChainSize - 1]) {
                // inverse flag
                repeated = false;

                // write repeated GNN_chain
                fputc(GNN_currentChainSize, GNN_output);
                fputc(GNN_chain[0], GNN_output);

                // refresh buffer
                memset(&GNN_chain, 0, 128);
                GNN_currentChainSize = 0;
            }
        } else {
            // if read 3 repeated symbols
            if (GNN_currentChainSize >= 2 && GNN_c == GNN_chain[GNN_currentChainSize - 1] && GNN_c == GNN_chain[GNN_currentChainSize - 2]) {
                repeated = true;

                // write current unrepeated GNN_chain if it exists
                if (GNN_currentChainSize >= 3) {
                    int chainSizeWithoutRepeats = GNN_currentChainSize - 2;
                    fputc(-chainSizeWithoutRepeats, GNN_output);
                    fwrite(GNN_chain, sizeof(signed char) * chainSizeWithoutRepeats, 1, GNN_output);

                    // refresh buffer
                    memset(&GNN_chain, 0, 128);

                    // fill repeated GNN_chain with two same bytes
                    GNN_chain[0] = GNN_c;
                    GNN_chain[1] = GNN_c;
                    GNN_currentChainSize = 2;
                }
            }
        }
        // remember last symbol
        GNN_chain[GNN_currentChainSize++] = GNN_c;

        // read one more symbol
        GNN_c = getc(GNN_input);

        // max length 127 for repeated GNN_chain, 128 for unrepeated GNN_chain
        int maxLength = repeated ? 127 : 128;
        // write GNN_chain to file if it exceed max size or get end of file
        if (GNN_currentChainSize == maxLength || feof(GNN_input)) {
            if (repeated) {
                fputc(GNN_currentChainSize, GNN_output);
                fputc(GNN_chain[0], GNN_output);
            } else {
                fputc(-GNN_currentChainSize, GNN_output);
                fwrite(GNN_chain, sizeof(signed char) * GNN_currentChainSize, 1, GNN_output);
            }

            memset(&GNN_chain, 0, 128);
            GNN_currentChainSize = 0;
        }
    }
    fclose(GNN_input);
    fclose(GNN_output);

    cout << "Created file " << GNN_outputFileName << endl;
    cout << "Compress successfully complete\n\n";

    return GNN_outputFileName;
}

string decompressFile(const char* inputFileName) {
    checkFileExist(inputFileName);

    // get restored file name without '.arh', if this file exist, than add (1) before extension
    string GNN_decompressedFileName = getRestoredFileName(inputFileName);

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(GNN_decompressedFileName.c_str(), "wb");

    cout << "Created file " << GNN_decompressedFileName << endl;

    signed char GNN_chain[128];
    signed char chainSize = getc(GNN_input);
    signed char repeatedSymbol;

    // in case of empty file
    if (chainSize == EOF) {
        fputc(chainSize, GNN_output);
        fclose(GNN_input);
        fclose(GNN_output);
        return GNN_decompressedFileName;
    }

    while (!feof(GNN_input)) {
        // restore repeated GNN_chain
        if (chainSize > 0) {
            repeatedSymbol = getc(GNN_input);
            for (int i = 0; i < chainSize; i++) {
                fputc(repeatedSymbol, GNN_output);
            }
        } else {
            // restore unrepeated GNN_chain
            fread(GNN_chain, sizeof(signed char) * (-chainSize), 1, GNN_input);
            fwrite(GNN_chain, sizeof(signed char) * (-chainSize), 1, GNN_output);
        }
        chainSize = getc(GNN_input);
    }

    fclose(GNN_input);
    fclose(GNN_output);

    cout << "Decompress successfully complete\n\n";

    return GNN_decompressedFileName;
}