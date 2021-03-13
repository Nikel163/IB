#include "utils.h"

using namespace std;

uint32_t* generateKey() {
    mt19937 GNN_gen;
    GNN_gen.seed(time(0));

    auto* GNN_k = new uint32_t[4];

    for (int i = 0; i < 4; i++) {
        char* GNN_buf = new char[4];
        for (int j = 0; j < 4; ++j) {
            GNN_buf[j] = GNN_gen() % 256;
        }
        memcpy(&GNN_k[i], GNN_buf, 4);
    }
    return GNN_k;
}

uint32_t* read16BytesFromFile(FILE* input) {
    auto* keyFromFile = new uint32_t[4];
    fread(keyFromFile, 16, 1, input);
    return keyFromFile;
}

void checkFileExist(const char* inputFileName) {
    if (_access(inputFileName, F_OK) != 0) {
        cerr << "File " << inputFileName << " not found\n";
        abort();
    }
}

size_t getFileSize(FILE* in) {
    fseek(in, 0, SEEK_END);
    size_t size = ftell(in);
    rewind(in);
    return size;
}

char* getEncFileName(const char* fileName) {
    char* newFileName = strdup(fileName);
    strcat(newFileName, ".enc");
    return newFileName;
}

char* getBase64FileName(const char* fileName) {
    char* newFileName = strdup(fileName);
    strcat(newFileName, ".b64");
    return newFileName;
}

void GNN_teaEncrypt(uint32_t* GNN_block, const uint32_t* GNN_k) {
    uint32_t GNN_delta = 0x9e3779b9,
             GNN_sum = 0;

    int GNN_n = 32;
    while(GNN_n--) {
        GNN_sum += GNN_delta;
        GNN_block[0] += (( GNN_block[1] << 4 ) + GNN_k[0] ) ^ (GNN_block[1] + GNN_sum ) ^ (( GNN_block[1] >> 5 ) + GNN_k[1] );
        GNN_block[1] += (( GNN_block[0] << 4 ) + GNN_k[2] ) ^ (GNN_block[0] + GNN_sum ) ^ (( GNN_block[0] >> 5 ) + GNN_k[3] );
    }
}

string getRestoredFileName(string& fileName) {
    // append (<number>) if file exists, i.e. file(1).txt
    string finalFileName = fileName;
    int GNN_i = 1;
    while (_access(finalFileName.c_str(), F_OK) == 0) {
        string GNN_number = string("(") + to_string(GNN_i++) + string(")");
        finalFileName = fileName;
        finalFileName.insert(finalFileName.rfind('.'), GNN_number);
    }

    return finalFileName;
}

string cutLastExtension(const std::string& fileName) {
    return fileName.substr(0, fileName.size() - 4);
};


void setHiddenInput(bool enable) {
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (enable) {
        mode &= ~ENABLE_ECHO_INPUT;
    } else {
        mode |= ENABLE_ECHO_INPUT;
    }

    SetConsoleMode(hStdin, mode);
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) {
        tty.c_lflag &= ~ECHO;
    } else {
        tty.c_lflag |= ~ECHO;
    }

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void printHelp() {
    cout << "Correct arguments: <key> <filename>, where <key> should be equal -e for encrypting"
         << endl << "or -d for decrypting" << endl;
}

void ask(const char* question) {
    cout << endl << question << endl;
    cout << "1 - yes, 2 - no" << endl;
}

bool parseAnswer() {
    cout << "Answer:";
    int ans;
    cin >> ans;
    if (ans != 1 && ans != 2) {
        while (true) {
            cout << "Wrong answer. Type 1 or 2:";
            cin >> ans;
            if (ans == 1 || ans == 2) {
                break;
            }
        }
    }
    cout << endl;

    return ans == 1 ? true : false;
}