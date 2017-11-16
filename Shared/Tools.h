#ifndef CONNECTIVITY_TOOLS_H
#define CONNECTIVITY_TOOLS_H
#include <cstring>

std::size_t intlen(int i);

void numberToCharArray(unsigned char *array, int number);
size_t insertNumberToArray(unsigned char *array, size_t offset, int number);

#endif //CONNECTIVITY_TOOLS_H
