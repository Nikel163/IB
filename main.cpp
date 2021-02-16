#include <iostream>
#include <ctime>
#include <random>
#include <fstream>
#include <cstring>

using namespace std;

void GNN_startEncrypt(const string& fileName);
void GNN_startDecrypt(const string& fileName, const string& keyFileName);

int main(int argc, char* argv[]) {
    if (argc > 2) {
        if (strcmp(argv[1], "-e") == 0) {
            GNN_startEncrypt(argv[2]);
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

void GNN_startEncrypt(const string& fileName) {
    // ���뢠��� 䠩�� � ������ ������
    ifstream GNN_in(fileName);
    if (GNN_in.is_open()) {
        // ���� ������ ��� ��஢��
        auto* GNN_v = new uint32_t[2];

        // ���뢠��� ������ �� 䠩��
        for (int i = 0; i < 2; i++) {
            char* buf = new char[4];
            GNN_in.read(buf, 4);
            memcpy(&GNN_v[i], buf, 4);
        }
        GNN_in.close();

        // ������� ��砩��� �ᥫ
        mt19937 GNN_gen;
        GNN_gen.seed(time(0));

        // 128-���� ����, ࠧ���� �� 4 ���
        auto* GNN_k = new uint32_t[4];
        // ��⮪ ��� ����� 䠩�� � ���箬
        ofstream GNN_keyFile("key.txt");

        for (int i = 0; i < 4; i++) {
            char* GNN_buf = new char[4];
            for (int j = 0; j < 4; j++) {
                // ������� ��砩���� ᨬ����
                GNN_buf[j] = GNN_gen() % 256;
            }
            // ������ 4 ᨬ����� � 䠩�
            GNN_keyFile << GNN_buf;
            // ��������� �� char* � uint32_t
            memcpy(&GNN_k[i], GNN_buf, 4);
        }
        GNN_keyFile.close();

        // -------- ��砫� ��஢���� ����� ������ ------------
        uint32_t GNN_delta = 0x9e3779b9,
                 GNN_sum = 0;

        int GNN_n = 32;
        while(GNN_n--) {
            GNN_sum += GNN_delta;
            GNN_v[0] += (( GNN_v[1] << 4 ) + GNN_k[0] ) ^ (GNN_v[1] + GNN_sum ) ^ (( GNN_v[1] >> 5 ) + GNN_k[1] );
            GNN_v[1] += (( GNN_v[0] << 4 ) + GNN_k[2] ) ^ (GNN_v[0] + GNN_sum ) ^ (( GNN_v[0] >> 5 ) + GNN_k[3] );
        }
        // -------- ����� ��஢���� ����� ������ ---------

        // ��� ����஢������ 䠩��
        string GNN_newFileName = fileName + ".enc";
        // ��⮪ ��� ����஢������ 䠩��
        ofstream GNN_encryptedFile(GNN_newFileName);
        // ������ ����஢������ 䠩��
        for (int i = 0; i < 2; i++) {
            char* GNN_buf = new char[4];
            memcpy(GNN_buf, &GNN_v[i], 4);
            GNN_encryptedFile << GNN_buf;
        }
        GNN_encryptedFile.close();
        cout << "������ 䠩� " << GNN_newFileName << endl;
        cout << "���஢���� ��諮 �ᯥ譮." << endl;
    } else {
        cerr << "���� " << fileName << " �� ������";
        abort();
    }
}

void GNN_startDecrypt(const string& fileName, const string& keyFileName) {
    ifstream GNN_in(fileName, ios::binary | ios::in);
    if (GNN_in.is_open()) {
        // ����� ��� ����஢��
        auto* GNN_v = new uint32_t[2];
        // ���뢠��� �� 䠩�� �� 4 ����
        for (int i = 0; i < 2; i++) {
            char* GNN_buf = new char[4];
            GNN_in.read(GNN_buf, 4);
            memcpy(&GNN_v[i], GNN_buf, 4);
        }
        GNN_in.close();

        // 128-���� ����, ࠧ���� �� 4 ���
        auto* GNN_k = new uint32_t[4];
        // ��⮪ 䠩��, � ���஬ ����ᠭ ����
        ifstream GNN_keyFile(keyFileName);
        if (GNN_keyFile.is_open()) {
            // ���뢠��� 32 ��� ����
            for (int i = 0; i < 4; i++) {
                char* GNN_buf = new char[4];
                GNN_keyFile.read(GNN_buf, 4);
                memcpy(&GNN_k[i], GNN_buf, 4);
            }
            cout << "���� �ᯥ譮 ��⠭ �� 䠩�� " << keyFileName << endl;
        } else {
            cerr << "���� " << keyFileName << " �� ������";
            abort();
        }

        // ----------- ��砫� ����஢���� ����� ������ -----------------
        uint32_t GNN_delta = 0x9e3779b9,
                 GNN_sum = 0xC6EF3720;

        int GNN_n = 32;
        while(GNN_n--) {
            GNN_v[1] -= (( GNN_v[0] << 4 ) + GNN_k[2] ) ^ (GNN_v[0] + GNN_sum ) ^ (( GNN_v[0] >> 5 ) + GNN_k[3] );
            GNN_v[0] -= (( GNN_v[1] << 4 ) + GNN_k[0] ) ^ (GNN_v[1] + GNN_sum ) ^ (( GNN_v[1] >> 5 ) + GNN_k[1] );
            GNN_sum -= GNN_delta;
        }
        // ------------ ����� ����஢���� ����� ������ -----------------

        // ����⠭������� ��室���� �������� 䠩��
        string GNN_newFileName = fileName.substr(0, fileName.size() - 4);

        // ��६����� ��� ����� ��᫥����� ��ਠ�� ��������  䠩�� � ��砥 ������ 䠩�� � ������ � GNN_newFileName
        string GNN_finalFileName = GNN_newFileName;

        // ����稪 ��� ᮧ����� ������ ��������
        int GNN_i = 1;
        // ����⨥ 䠩��
        FILE *GNN_fp = fopen(GNN_finalFileName.c_str(), "r");
        // �஢�ઠ �� ����稥 䠩�� � ⠪�� �������
        while (GNN_fp != NULL) {
            // ���������� � �������� 䠩�� ��ப� ���� (1)
            string GNN_number = string("(") + to_string(GNN_i++) + string(")");
            // ���樠������ �������� ��室���� 䠩��
            GNN_finalFileName = GNN_newFileName;
            // ��⠢�� � �������� 䠩�� ����, �.�. file.txt -> file(1).txt
            GNN_finalFileName.insert(GNN_finalFileName.find('.'), GNN_number);
            // �஢�ઠ �� ����稥 䠩�� � ���� ������
            GNN_fp = fopen(GNN_finalFileName.c_str(), "r");
        }
        // ��⮪ ��� ����� ����஢������ 䠩��
        ofstream GNN_decryptedFile(GNN_finalFileName);

        for (int i = 0; i < 2; i++) {
            char* buf = new char[4];
            memcpy(buf, &GNN_v[i], 4);
            GNN_decryptedFile << buf;
        }
        GNN_decryptedFile.close();
        cout << "������ 䠩� " << GNN_finalFileName << endl;
        cout << "����஢���� ��諮 �ᯥ譮." << endl;
    } else {
        cerr << "���� " << fileName << " �� ������";
        abort();
    }
}
