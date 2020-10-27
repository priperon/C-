/*
	Code copied from Garth Santor's code during classes
*/
#define _CRT_DEBUG_MAP_ALLOC
#include <crtdbg.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

typedef double array_value_t;


typedef struct array_t_tag
{
	array_value_t* data;
	size_t nSize;
	size_t nCapacity;
} array_t;


array_t array();	// function prototype (or signature)
bool array_push_back(array_t* pArray, array_value_t value);
void array_free(array_t* rolls);