#include "private_libedifact.h"

char* create_dynamic_string(char* data, size_t length) {
    char* string = malloc(length + 1);
    strncpy_s(string, length + 1, data, length);
    string[length] = '\0';
    return string;
}