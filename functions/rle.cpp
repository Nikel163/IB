#include "rle.h"

using namespace std;

void compressFile(FILE* GNN_input, FILE* GNN_output) {

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

    cout << "Compress successfully complete\n\n";
}

void decompressFile(FILE* GNN_input, FILE* GNN_output) {
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

    cout << "Decompress successfully complete\n\n";
}