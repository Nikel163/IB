#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../utils/utils.h"

void startOFB(FILE* GNN_input, FILE* GNN_output, const std::string& password, bool isEncrypting, bool withSessionKey);

#endif