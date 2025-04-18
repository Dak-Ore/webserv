#pragma once

#include <unistd.h>
#include <stdexcept>

// TODO really?
size_t my_strlen(char const* const str);
char* my_strdup(char const* const str);
char** strarraydup(char const* const array[]);
char* concat(char const* const a, char const* const b);

void forkexec(int inout[2], char* const argv[], char* const envp[]);
