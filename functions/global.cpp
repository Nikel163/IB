#include "global.h"

using namespace std;

void makeItBeautiful(const char* inputFileName, const string& password, bool isArchiving,
                     bool withSessionKey, bool isNeedBase64Encode) {

    string currentFileName(inputFileName);

    if (isArchiving) {
        currentFileName = compressFile(inputFileName);
    }

    currentFileName = GNN_startOFB(currentFileName.c_str(), getEncFileName(currentFileName.c_str()), password, true, withSessionKey).c_str();

    if (isNeedBase64Encode) {
        base64encode(currentFileName.c_str(), getBase64FileName(currentFileName.c_str()));
    }
}

void bringMeBack(const string& inputFileName, const string& password, bool withSessionKey) {
    string lastExtension = inputFileName.substr(inputFileName.size() - 4, 4);
    string fileName = inputFileName;
    if (lastExtension == ".b64") {
        fileName = base64decode(inputFileName.c_str(), cutLastExtension(inputFileName).c_str());
    }

    lastExtension = fileName.substr(fileName.size() - 8, 4);

    string outputFileName = lastExtension == ".arh" ? cutLastExtension(fileName) : getRestoredFileName(fileName.c_str());

    fileName = GNN_startOFB(fileName.c_str(), outputFileName.c_str(), password, false, withSessionKey);

    if (lastExtension == ".arh") {
        decompressFile(fileName.c_str());
    }
}