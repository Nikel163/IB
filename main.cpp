#include "functions/ofb.h"
#include "functions/rle.h"
#include "functions/base64.h"

using namespace std;

int main(int argc, char* argv[]) {
    // argument parsing
    if (argc > 3) {
        if (strcmp(argv[1], "-c") == 0) {
            base64encode(argv[2], argv[3]);
        } else if (strcmp(argv[1], "-d") == 0) {
            base64decode(argv[2], argv[3]);
        } else {
            cerr << "Invalid arguments";
            abort();
        }
    } else {
        cerr << "Not enough arguments";
        abort();
    }
}




