/*
	Code copied from Garth Santor's code during classes
*/
#include "stats_t.h"
#include <assert.h> 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>


// from Garth Santor's code during classes
unsigned const DEFAULT_CAP = 8;
int const OK = 1;
int const FAIL = 0;

// from Garth Santor's code during classes
#ifdef _M_X64
void* const DBG_PTR = (void* const)0xcccccccccccccccc; // each hex character perfectly matches a nibble; each cccc is equivalent to two bytes. 
//If it's Microsoft 32 b version it'll use the 16 bytes for the pointer, else it'll use 8.
#else
void* const DBG_PTR = (void* const)0xcccccccc;
#endif

/*Make an empty array object*/
// from Garth Santor's code during classes
array_t array()
{
	array_t arr;
	arr.data = NULL;
	arr.nCapacity = 0;
	arr.nSize = 0; 
	return arr;
}

/*  Method: array_push_back
	Accepts: an array of type double
	Retirns: boolean
	Purpose: add an element to the end of the array.
	More memory is allocated if needed.*/
	// from Garth Santor's code during classes
bool array_push_back(array_t* pArray, array_value_t value)
{
	
	assert(pArray != NULL && "pArray is NULL, should point to an array_t structure"); // if my pointer to the array object points to nothing the function won't be called
	assert(pArray != DBG_PTR && "pArray has not been initialized."); // checks if the pointer has been initialized
	assert(pArray->data != DBG_PTR && "The array_t object has not been initialized, use array()"); // checks if the pArray object has been initialized
	assert(pArray->nSize <= pArray->nCapacity && "array size should never exceed capacity - corruption!");

	// Expand if necessary
	if (pArray->nCapacity == pArray->nSize)
	{
		size_t newCapacity = pArray->nCapacity * 2;
		if (newCapacity == 0) ++newCapacity;
		// realloc will relocate things to a new address so if it goes to a new location we need to change the address to the right one
		array_value_t* pNewBlock = realloc(pArray->data, newCapacity * sizeof(array_value_t));
		if (pNewBlock == NULL)
			return false;

		pArray->nCapacity = newCapacity;
		pArray->data = pNewBlock;
	}

	// store the element
	pArray->data[pArray->nSize++] = value;
	return true;
}


/*  Method:  array_free
	Accepts: an array
	Retirns: void
	Purpose: empties the array.*/
	// from Garth Santor's code during classes
void array_free(array_t* pArray)
{
	assert(pArray->nSize <= pArray->nCapacity && "array size should never exceed capacity - corruption!");

	free(pArray->data);
	*pArray = array(); 	// puts back the object to the starting point
}