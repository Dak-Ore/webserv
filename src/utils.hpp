#pragma once

#include <unistd.h>

/**
 * Concatenate two NUL-ended arrays.
 * 
 * The return value is created with new[].
 */
template <typename T>
T* concat(T const* a, T const* b);

/**
 * Get the size of a NUL-ended list.
 */
template <typename T>
size_t len(T const* a);

/**
 * Clone a list.
 *
 * The return value is created with new[].
 */
template <typename T>
T* dupl(T const* a);

/**
 * Deep-clone a list that contains other lists.
 * 
 * The return value and all its items are created with new[].
 */
template <typename T>
T** arraydupl(T const* const* list);

/**
 * Create a subprocess from 'argv'.
 * 
 * - inout[0] and inout[1] will be set to two open file descriptors
 *   respectively the subprocess' stdout and stdin.
 * - argv are the arguments that will be given to the subprocess.
 *   argv[0] must be a path to the executable to call.
 * - envp is the environment to the subprocess.
 */
void forkexec(int inout[2], char const* const* argv, char const* const* envp);

#include "utils.tpp"
