#pragma once

#include <unistd.h>

size_t my_strlen(char const* const str);

char* my_strdup(char const* const str);

char** strarraydup(char const* const array[]);