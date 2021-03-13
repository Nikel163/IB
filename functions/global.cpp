#include "global.h"

using namespace std;

void makeItBeautiful(char* inputFileName, const string& password, bool isArchiving,
                     bool withSessionKey, bool isNeedBase64Encode) {
    checkFileExist(inputFileName);

    FILE* input = fopen(inputFileName, "rb");
    FILE* output;
    FILE* tempFile = tmpfile();

    if (isArchiving) {
        compressFile(input, tempFile);
        rewind(tempFile);
        if (isNeedBase64Encode) {
            FILE* tempFile2 = tmpfile();
            startOFB(tempFile, tempFile2, password, true, withSessionKey);
            rewind(tempFile2);

            output = fopen(strcat(inputFileName, ".arh.enc.b64"), "wb");
            base64encode(tempFile2, output);

            fclose(tempFile2);
        } else {
            output = fopen(strcat(inputFileName, ".arh.enc"), "wb");
            startOFB(tempFile, output, password, true, withSessionKey);
        }
    } else {
        if (isNeedBase64Encode) {
            startOFB(input, tempFile, password, true, withSessionKey);
            rewind(tempFile);

            output = fopen(strcat(inputFileName, ".enc.b64"), "wb");
            base64encode(tempFile, output);
        } else {
            output = fopen(strcat(inputFileName, ".enc"), "wb");
            startOFB(input, output, password, true, withSessionKey);
        }
    }
    fclose(input);
    fclose(output);
    fclose(tempFile);
}

void bringMeBack(const string& fileName, const string& password, bool withSessionKey) {
    checkFileExist(fileName.c_str());

    string lastExtension = fileName.substr(fileName.size() - 4, 4);

    bool decode = lastExtension == ".b64";
    bool unarchive;

    if (decode) {
        unarchive = fileName.size() > 12 ? fileName.substr(fileName.size() - 12, 4) == ".arh" : false;
    } else {
        unarchive = fileName.size() > 8 ? fileName.substr(fileName.size() - 8, 4) == ".arh" : false;
    }

    int skip;
    if (decode && unarchive) {
        skip = 12;
    } else if (decode ^ unarchive) {
        skip = 8;
    } else {
        skip = 4;
    }

    string sourceFileName = fileName.substr(0, fileName.size() - skip);
    string outputFileName = getRestoredFileName(sourceFileName);

    FILE* input = fopen(fileName.c_str(), "rb");
    FILE* output = fopen(outputFileName.c_str(), "wb");
    FILE* tempFile = tmpfile();

    if (decode) {
        base64decode(input, tempFile);
        rewind(tempFile);
        if (unarchive) {
            FILE* tempFile2 = tmpfile();
            startOFB(tempFile, tempFile2, password, false, withSessionKey);
            rewind(tempFile2);
            decompressFile(tempFile2, output);
            fclose(tempFile2);
        } else {
            startOFB(tempFile, output, password, false, withSessionKey);
        }
    } else {
        if (unarchive) {
            startOFB(input, tempFile, password, false, withSessionKey);
            rewind(tempFile);
            base64encode(tempFile, output);
        } else {
            startOFB(input, output, password, false, withSessionKey);
        }
    }
    fclose(input);
    fclose(output);
    fclose(tempFile);
}