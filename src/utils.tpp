#pragma once

#include "utils.hpp"

template <typename T>
size_t len(T const* a)
{
	size_t r = 0;
	while (a[r] != 0)
		r++;
	return r;
}

template <typename T>
T* dupl(T const* a)
{
	char* r = new T[len(a) + 1];
	size_t i;
	for (i = 0; a[i]; i++)
		r[i] = a[i];
	r[i] = '\0';
	return r;
}

template <typename T>
T* concat(T const *a, T const* b)
{
	T* r = new T[len(a) + len(b) + 1];
	size_t i = 0;
	for (size_t j = 0; a[j] != 0; j++) {
		r[i] = a[j];
		i++;
	}
	for (size_t j = 0; b[j] != 0; j++) {
		r[i] = b[j];
		i++;
	}
	r[i] = 0;
	return r;
}

template <typename T>
T** arraydupl(T const* const* a)
{
	size_t size = 0;
	while (a[size])
		size++;
	T** r = new T*[size + 1];
	size_t i;
	for (i = 0; i < size; i++)
		r[i] = dupl(a[i]);
	r[i] = NULL;
	return r;
}
