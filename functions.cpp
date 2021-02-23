#include <io.h>
#include "functions.h"

using namespace std;

void GNN_startEncrypt(const char* inputFileName, const char* outputFileName, uint32_t* GNN_k) {
    FILE* GNN_in = fopen(inputFileName, "rb");

    if (_access(inputFileName, F_OK) != 0) {
        cerr << "Файл " << inputFileName << "не найден\n";
        abort();
    }

    FILE* GNN_out = fopen(outputFileName, "wb");

    uint32_t GNN_initializationVector[2] = {0xBABAFFFF, 0xFFFFAAAA};
    uint32_t GNN_block[2];
    uint32_t GNN_blockEncrypted[2];

    uint32_t GNN_blockSize = sizeof(GNN_block);

    long GNN_fileSize = getFileSize(GNN_in);
    uint32_t GNN_tailSize = GNN_fileSize % GNN_blockSize;
    uint32_t GNN_blocksNumber = GNN_fileSize / GNN_blockSize;

    cout << "Размер файла в байтах: " << GNN_fileSize << endl;
    cout << "Размер остаточного блока в байтах: " << GNN_tailSize << endl;
    cout << "Количество блоков: " << GNN_blocksNumber << endl;

    while (GNN_blocksNumber--) {
        // блок данных для шифровки
        fread(GNN_block, GNN_blockSize, 1, GNN_in);

        GNN_teaEncrypt(GNN_initializationVector, GNN_k);
        //XOR зашифрованного блока с накопленной гаммой
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }

        fwrite(GNN_blockEncrypted, GNN_blockSize, 1, GNN_out);
    }

    if (GNN_tailSize > 0) {
        fread(GNN_block, GNN_blockSize, 1, GNN_in);

        GNN_teaEncrypt(GNN_initializationVector, GNN_k);
        for (int i = 0; i < 2; i++) {
            GNN_blockEncrypted[i] = GNN_block[i] ^ GNN_initializationVector[i];
        }

        fwrite(GNN_blockEncrypted, GNN_tailSize, 1, GNN_out);
    }

    cout << "Шифрование прошло успешно." << endl;

}

void GNN_startDecrypt(const char* encryptedFileName, const char* keyFileName) {
    // проверка на наличие файла с ключом
    if (_access(keyFileName, F_OK) != 0) {
        cerr << "Файл " << keyFileName << "не найден\n";
        abort();
    }

    // считывание ключа из файла
    uint32_t GNN_k[4];
    FILE* keyFile = fopen(keyFileName, "rb");
    fread(GNN_k, 16, 1, keyFile);

    // начало дешифровки
    GNN_startEncrypt(encryptedFileName, getRestoredFileName(encryptedFileName).c_str(), GNN_k);

    cout << "Дешифрование прошло успешно." << endl;

}