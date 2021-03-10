#include "functions/global.h"

using namespace std;

int main(int argc, char* argv[]) {
    setHiddenInput(true);
    cout << "Enter password:\n";
    string password;
    cin >> password;
    setHiddenInput(false);

    if (argc == 3) {
        if (strcmp(argv[1], "-e") == 0) {
            ask("Do you want to archive file?");
            bool isArchiving = parseAnswer();

            ask("Do you want to encrypt with session key?");
            bool withSessionKey = parseAnswer();

            ask("Do you want base64 encoding?");
            bool isNeedToBase64 = parseAnswer();
            makeItBeautiful(argv[2], password, isArchiving, withSessionKey, isNeedToBase64);
        } else if (strcmp(argv[1], "-d") == 0) {
            ask("File was encrypted with sessionKey?");
            bool withSessionKey = parseAnswer();

            bringMeBack(argv[2], password, withSessionKey);
        } else {
            cerr << "Invalid arguments" << endl;
            printHelp();
            abort();
        }
    } else {
        cerr << "Not enough arguments" << endl;
        printHelp();
        abort();
    }
}




