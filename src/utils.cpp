#include "utils.hpp"

size_t my_strlen(char const* const str)
{
	size_t r = 0;
	while (str[r])
		r++;
	return r;
}

char* my_strdup(char const* const str)
{
	char* r = new char[my_strlen(str) + 1];
	size_t i;
	for (i = 0; str[i]; i++)
		r[i] = str[i];
	r[i] = '\0';
	return r;
}

char** strarraydup(char const* const array[])
{
	size_t size = 0;
	while (array[size])
		size++;
	char** r = new char*[size + 1];
	size_t i;
	for (i = 0; i < size; i++)
		r[i] = my_strdup(array[i]);
	r[i] = NULL;
	return r;
}
