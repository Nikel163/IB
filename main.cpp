#include <iostream>
#include "functions.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 2) {
        if (strcmp(argv[1], "-e") == 0) {
            GNN_startEncrypt(argv[2]);
        } else if (strcmp(argv[1], "-d") == 0 && argv[3]) {
            GNN_startDecrypt(argv[2], argv[3]);
        } else {
            cerr << "Некорректный ввод аргументов";
            abort();
        }
    } else {
        cerr << "Не хватает аргументов";
        abort();
    }
}




