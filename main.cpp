#include "functions.h"

using namespace std;

int main(int argc, char* argv[]) {
    // ࠧ��� ��㬥�⮢
    if (argc > 2) {
        if (strcmp(argv[1], "-e") == 0) {
            uint32_t* GNN_k = generateKey();
            writeKeyToFile(GNN_k);
            GNN_startEncrypt(argv[2], getEncFileName(argv[2]), GNN_k);
        } else if (strcmp(argv[1], "-d") == 0 && argv[3]) {
            GNN_startDecrypt(argv[2], argv[3]);
        } else {
            cerr << "�����४�� ���� ��㬥�⮢";
            abort();
        }
    } else {
        cerr << "�� 墠⠥� ��㬥�⮢";
        abort();
    }
}




