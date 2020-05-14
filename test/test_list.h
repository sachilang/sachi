#ifndef TEST_LIST_H
#define TEST_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/list.h"
#include "sachi/object/bool.h"

#ifdef __cplusplus
}
#endif

void test_list()
{
#define INITIAL_CAPACITY 0
#define MAX_CAPACITY 5

	Sachi_Interpreter* Interpreter = Sachi_NewInterpreter();
	assert(Interpreter != NULL);

	// Create list
	Sachi_Object* List = Sachi_ListType.New(Interpreter);
	assert(List != NULL);
	sachi_size_t Size = SachiList_Size(List);
	assert(Size == 0);
	assert(Sachi_IsTrue(SachiList_Empty(List)));
	sachi_size_t Capacity = SachiList_Capacity(List);
	assert(Capacity == INITIAL_CAPACITY);
	std::cout << "Initial list size=" << Size << std::endl;
	std::cout << "Initial list capacity=" << Capacity << std::endl;

	// Fill list
	for (sachi_size_t I = 1; I <= MAX_CAPACITY; ++I)
	{
		SachiList_Push(List, (Sachi_Object*)Sachi_True);
		Size = SachiList_Size(List);
		assert(Size == I);
		std::cout << "After push size=" << Size << std::endl;
	}

	assert(Sachi_IsFalse(SachiList_Empty(List)));

	// Set by index
	assert(SachiList_SetItem(List, Size - 1, (Sachi_Object*)Sachi_False) == SACHI_OK);
	assert(SachiList_SetItem(List, Size, (Sachi_Object*)Sachi_False) == SACHI_ERROR);

	// Get by index
	assert(SachiList_GetItem(List, Size - 1) != NULL);
	assert(SachiList_GetItem(List, Size) == NULL);

	// Front and back
	assert(SachiList_Front(List) != NULL);
	assert(SachiList_Back(List) != NULL);

	// Empty list
	for (sachi_size_t I = MAX_CAPACITY; I >= 1; --I)
	{
		SachiList_Pop(List);
		Size = SachiList_Size(List);
		assert(Size == (I - 1));
		std::cout << "After pop size=" << Size << std::endl;
	}

	assert(Sachi_IsTrue(SachiList_Empty(List)));
	assert(SachiList_Capacity(List) == MAX_CAPACITY);

	// List already empty
	SachiList_Pop(List);
	Size = SachiList_Size(List);
	assert(Size == 0);
	std::cout << "After pop size=" << Size << std::endl;

	// Destroy
	Sachi_ListType.Delete(List);

	Sachi_DeleteInterpreter(Interpreter);
}

#endif