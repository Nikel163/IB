#include "functions.h"

using namespace std;

// функция для получения размера файла
int getFileSize(ifstream &in) {
    // постановка указателя в конец файла
    in.seekg(0, in.end);
    // получение размера файла
    int size = in.tellg();
    // постановка указателя в начало файла
    in.seekg(0, in.beg);

    return size;
}

uint32_t* read8BytesFromFile(ifstream &GNN_in) {
    auto* GNN_v = new uint32_t[2];

    // считывание данных из файла по 4 байта
    for (int i = 0; i < 2; i++) {
        char* buf = new char[4];
        GNN_in.read(buf, 4);
        memcpy(&GNN_v[i], buf, 4);
    }

    return GNN_v;
}

uint32_t* generateKey() {
    // генератор случайных чисел
    mt19937 GNN_gen;
    GNN_gen.seed(time(0));

    // 128-битный ключ, разбитый на 4 части
    auto* GNN_k = new uint32_t[4];

    cout << "Запись ключа: ";
    for (int i = 0; i < 4; i++) {
        char* GNN_buf = new char[4];
        for (int j = 0; j < 4; j++) {
            // генерация случайного символа
            GNN_buf[j] = GNN_gen() % 256;
            cout << GNN_buf[j];
        }
        // запись 4 символов в файл
        // конвертация из char* в uint32_t
        memcpy(&GNN_k[i], GNN_buf, 4);
    }
    cout << endl;
    return GNN_k;
}

void writeKeyToFile(uint32_t* GNN_k) {

}

void GNN_startEncrypt(const string& fileName) {
    // считывание файла с блоком данных
    ifstream GNN_in(fileName, ios::binary);
    if (GNN_in.is_open()) {

        uint32_t* GNN_k = generateKey();
        writeKeyToFile(GNN_k);

        int fileSize = getFileSize(GNN_in);
        int tailSize = fileSize % 8;
        int blocksNumber = fileSize / 8;
        cout << "File size: " << fileSize << endl;
        cout << "Tail size: " << tailSize << endl;
        cout << "Block number: " << blocksNumber << endl;

        while (blocksNumber--) {
            // блок данных для шифровки
            uint32_t* GNN_v = read8BytesFromFile(GNN_in);


        }

        GNN_in.close();

        // генератор случайных чисел
        mt19937 GNN_gen;
        GNN_gen.seed(time(0));

        // 128-битный ключ, разбитый на 4 части
        auto* GNN_k = new uint32_t[4];
        // поток для записи файла с ключом
        ofstream GNN_keyFile("key.txt");

        cout << "Запись ключа: ";
        for (int i = 0; i < 4; i++) {
            char* GNN_buf = new char[4];
            for (int j = 0; j < 4; j++) {
                // генерация случайного символа
                GNN_buf[j] = GNN_gen() % 256;
                GNN_keyFile << GNN_buf[j];
                cout << GNN_buf[j];
            }
            // запись 4 символов в файл
            // конвертация из char* в uint32_t
            memcpy(&GNN_k[i], GNN_buf, 4);
        }
        cout << endl;
        GNN_keyFile.close();

        // -------- начало шифрования блока данных ------------
        uint32_t GNN_delta = 0x9e3779b9,
                GNN_sum = 0;

        int GNN_n = 32;
        while(GNN_n--) {
            GNN_sum += GNN_delta;
            GNN_v[0] += (( GNN_v[1] << 4 ) + GNN_k[0] ) ^ (GNN_v[1] + GNN_sum ) ^ (( GNN_v[1] >> 5 ) + GNN_k[1] );
            GNN_v[1] += (( GNN_v[0] << 4 ) + GNN_k[2] ) ^ (GNN_v[0] + GNN_sum ) ^ (( GNN_v[0] >> 5 ) + GNN_k[3] );
        }
        // -------- конец шифрования блока данных ---------

        // имя зашифрованного файла
        string GNN_newFileName = fileName + ".enc";
        // поток для зашифрованного файла
        ofstream GNN_encryptedFile(GNN_newFileName);
        // запись зашифрованного файла
        for (int i = 0; i < 2; i++) {
            char* GNN_buf = new char[4];
            memcpy(GNN_buf, &GNN_v[i], 4);
            for (int j = 0; j < 4; j++) {
                GNN_encryptedFile << GNN_buf[j];
            }
        }
        GNN_encryptedFile.close();
        cout << "Создан файл " << GNN_newFileName << endl;
        cout << "Шифрование прошло успешно." << endl;
    } else {
        cerr << "Файл " << fileName << " не найден";
        abort();
    }
}

void GNN_startDecrypt(const string& fileName, const string& keyFileName) {
    ifstream GNN_in(fileName, ios::binary | ios::in);
    if (GNN_in.is_open()) {
        // данные для дешифровки
        auto* GNN_v = new uint32_t[2];
        // считывание из файла по 4 байта
        for (int i = 0; i < 2; i++) {
            char* GNN_buf = new char[4];
            GNN_in.read(GNN_buf, 4);
            memcpy(&GNN_v[i], GNN_buf, 4);
        }
        GNN_in.close();

        // 128-битный ключ, разбитый на 4 части
        auto* GNN_k = new uint32_t[4];
        // поток файла, в котором записан ключ
        ifstream GNN_keyFile(keyFileName);
        if (GNN_keyFile.is_open()) {
            // считывание 32 бит ключа
            for (int i = 0; i < 4; i++) {
                char* GNN_buf = new char[4];
                GNN_keyFile.read(GNN_buf, 4);
                memcpy(&GNN_k[i], GNN_buf, 4);
            }
            cout << "Ключ успешно считан из файла " << keyFileName << endl;
        } else {
            cerr << "Файл " << keyFileName << " не найден";
            abort();
        }

        // ----------- начало дешифрования блока данных -----------------
        uint32_t GNN_delta = 0x9e3779b9,
                GNN_sum = 0xC6EF3720;

        int GNN_n = 32;
        while(GNN_n--) {
            GNN_v[1] -= (( GNN_v[0] << 4 ) + GNN_k[2] ) ^ (GNN_v[0] + GNN_sum ) ^ (( GNN_v[0] >> 5 ) + GNN_k[3] );
            GNN_v[0] -= (( GNN_v[1] << 4 ) + GNN_k[0] ) ^ (GNN_v[1] + GNN_sum ) ^ (( GNN_v[1] >> 5 ) + GNN_k[1] );
            GNN_sum -= GNN_delta;
        }
        // ------------ конец дешифрования блока данных -----------------

        // восстановление исходного названия файла
        string GNN_newFileName = fileName.substr(0, fileName.size() - 4);

        // переменная для записи последнего варианта названия  файла в случае наличия файла с именем в GNN_newFileName
        string GNN_finalFileName = GNN_newFileName;

        // счётчик для создания нового названия
        int GNN_i = 1;
        // открытие файла
        FILE *GNN_fp = fopen(GNN_finalFileName.c_str(), "r");
        // проверка на наличие файла с таким имененм
        while (GNN_fp != NULL) {
            // добавление в название файла строки вида (1)
            string GNN_number = string("(") + to_string(GNN_i++) + string(")");
            // инициализация названия исходного файла
            GNN_finalFileName = GNN_newFileName;
            // вставка в название файла цифры, т.е. file.txt -> file(1).txt
            GNN_finalFileName.insert(GNN_finalFileName.find('.'), GNN_number);
            // проверка на наличие файла с новым именем
            GNN_fp = fopen(GNN_finalFileName.c_str(), "r");
        }
        // поток для записи дешифрованного файла
        ofstream GNN_decryptedFile(GNN_finalFileName);

        for (int i = 0; i < 2; i++) {
            char* buf = new char[4];
            memcpy(buf, &GNN_v[i], 4);
            for (int j = 0; j < 4; j++) {
                GNN_decryptedFile << buf[j];
            }
        }
        GNN_decryptedFile.close();
        cout << "Создан файл " << GNN_finalFileName << endl;
        cout << "Дешифрование прошло успешно." << endl;
    } else {
        cerr << "Файл " << fileName << " не найден";
        abort();
    }
}