#include "functions/ofb.h"
#include "functions/rle.h"

using namespace std;

int main(int argc, char* argv[]) {
    // argument parsing
    if (argc > 2) {
        if (strcmp(argv[1], "-c") == 0) {
            compressFile(argv[2]);
        } else if (strcmp(argv[1], "-d") == 0) {
            decompressFile(argv[2]);
        } else {
            cerr << "Invalid arguments";
            abort();
        }
    } else {
        cerr << "Not enough arguments";
        abort();
    }
}




