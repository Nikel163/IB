#ifndef BASE64_H
#define BASE64_H

#include "../utils/utils.h"

void base64encode(FILE* GNN_input, FILE* GNN_output);
void base64decode(FILE* GNN_input, FILE* GNN_output);

#endif