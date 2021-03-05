#include "rle.h"

using namespace std;

void compressFile(const char* inputFileName) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    string outputFileName(inputFileName);
    outputFileName.append(".arh");

    FILE* input = fopen(inputFileName, "rb");
    FILE* output = fopen(outputFileName.c_str(), "wb");

    cout << "Created file " << outputFileName << endl;

    signed char c = getc(input);

    // in case empty file
    if (feof(input)) {
        fputc(c, output);
        fclose(input);
        fclose(output);
        return;
    }

    // buffer for chain
    signed char chain[128];
    int currentChainSize = 0;

    // remember first symbol
    chain[currentChainSize] = c;

    // flag for different logic
    bool repeated = false;

    while (c != EOF) {
        // if reading same symbols
        if (repeated) {
            // if get another symbol
            if (currentChainSize >= 2 && c != chain[currentChainSize - 1]) {
                // inverse flag
                repeated = false;

                // write repeated chain
                fputc(currentChainSize, output);
                fputc(chain[0], output);

                // refresh buffer
                memset(&chain, 0, 128);
                currentChainSize = 0;
            }

        } else {
            // if read 3 repeated symbols
            if (currentChainSize >= 2 && c == chain[currentChainSize - 1] && c == chain[currentChainSize - 2]) {
                repeated = true;

                // write current unrepeated chain if exist
                if (currentChainSize >= 3) {
                    int chainSizeWithoutRepeats = currentChainSize - 2;
                    fputc(-chainSizeWithoutRepeats, output);
                    fwrite(chain, sizeof(signed char) * chainSizeWithoutRepeats, 1, output);

                    // refresh buffer
                    memset(&chain, 0, 128);

                    // fill repeated chain with two same bytes
                    chain[0] = c;
                    chain[1] = c;
                    currentChainSize = 2;
                }
            }
        }
        // remember last symbol
        chain[currentChainSize++] = c;

        // read one more symbol
        c = getc(input);

        // max length 127 for repeated chain, 128 for unrepeated chain
        int maxLength = repeated ? 127 : 128;

        // write chain to file if it exceed max size or get end of file
        if (currentChainSize == maxLength || c == EOF) {
            //
            if (repeated) {
                fputc(currentChainSize, output);
                fputc(chain[0], output);
            } else {
                fputc(-currentChainSize, output);
                fwrite(chain, sizeof(signed char) * currentChainSize, 1, output);
            }

            memset(&chain, 0, 128);
            currentChainSize = 0;
        }
    }
    cout << "Compress successfully complete" <<std::endl;
    fclose(input);
    fclose(output);
}

void decompressFile(const char* inputFileName) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }

    // get restored file name without '.arh', if this file exist, than add (1) before extension
    string decompressedFileName = getRestoredFileName(inputFileName);

    FILE* input = fopen(inputFileName, "rb");
    FILE* output = fopen(decompressedFileName.c_str(), "wb");

    cout << "Created file " << decompressedFileName << endl;

    signed char chain[128];
    signed char chainSize = getc(input);
    signed char repeatedSymbol;

    // in case of empty file
    if (chainSize == EOF) {
        fputc(chainSize, output);
        fclose(input);
        fclose(output);
        return;
    }

    while (!feof(input)) {
        // restore repeated chain
        if (chainSize > 0) {
            repeatedSymbol = getc(input);
            for (int i = 0; i < chainSize; i++) {
                fputc(repeatedSymbol, output);
            }
        } else {
            // restore unrepeated chain
            fread(chain, sizeof(signed char) * (-chainSize), 1, input);
            fwrite(chain, sizeof(signed char) * (-chainSize), 1, output);
        }
        chainSize = getc(input);
    }

    fclose(input);
    fclose(output);
}