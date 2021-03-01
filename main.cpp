#include "functions/functions.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Enter password: \n";
    setHiddenInput(true);
    string password; cin >> password;
    setHiddenInput(false);

    // argument parsing
    if (argc > 2) {
        if (strcmp(argv[1], "-e") == 0) {
            GNN_start(argv[2], getEncFileName(argv[2]), generateKey(), password, true);
            cout << "Encryption is successful." << endl;
        } else if (strcmp(argv[1], "-d") == 0) {
            // read key from first 16 bytes from file
            uint32_t keyFromFile[4];
            FILE* encryptedFile = fopen(argv[2], "rb");
            fread(keyFromFile, 16, 1, encryptedFile);
            fclose(encryptedFile);

            GNN_start(argv[2], getRestoredFileName(argv[2]).c_str(), keyFromFile, password, false);
            cout << "Decryption is successful." << endl;
        } else {
            cerr << "Invalid arguments";
            abort();
        }
    } else {
        cerr << "Not enough arguments";
        abort();
    }
}




