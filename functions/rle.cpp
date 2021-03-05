#include "rle.h"

using namespace std;

void compressFile(const char* inputFileName) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    string GNN_outputFileName(inputFileName);
    GNN_outputFileName.append(".arh");

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(GNN_outputFileName.c_str(), "wb");

    cout << "Created file " << GNN_outputFileName << endl;

    signed char GNN_c = getc(GNN_input);

    // in case empty file
    if (feof(GNN_input)) {
        fputc(GNN_c, GNN_output);
        fclose(GNN_input);
        fclose(GNN_output);
        return;
    }

    // buffer for chain
    signed char GNN_chain[128];
    int GNN_currentChainSize = 0;

    // remember first symbol
    GNN_chain[GNN_currentChainSize] = GNN_c;

    // flag for different logic
    bool repeated = false;

    while (GNN_c != EOF) {
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
        if (GNN_currentChainSize == maxLength || GNN_c == EOF) {
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
    cout << "Compress successfully complete\n";
    fclose(GNN_input);
    fclose(GNN_output);
}

void decompressFile(const char* inputFileName) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    // get restored file name without '.arh', if this file exist, than add (1) before extension
    string decompressedFileName = getRestoredFileName(inputFileName);

    FILE* GNN_input = fopen(inputFileName, "rb");
    FILE* GNN_output = fopen(decompressedFileName.c_str(), "wb");

    cout << "Created file " << decompressedFileName << endl;

    signed char GNN_chain[128];
    signed char chainSize = getc(GNN_input);
    signed char repeatedSymbol;

    // in case of empty file
    if (chainSize == EOF) {
        fputc(chainSize, GNN_output);
        fclose(GNN_input);
        fclose(GNN_output);
        return;
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

    cout << "Decompress successfully complete\n";
}