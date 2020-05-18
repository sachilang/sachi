#ifndef TEST_LIST_H
#define TEST_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/list.h"

#ifdef __cplusplus
}
#endif

void test_list(Sachi_Interpreter* InInterpreter)
{
#define INITIAL_CAPACITY 0
#define MAX_CAPACITY 5

	// Create list
	Sachi_Object* List = Sachi_ListType.New(InInterpreter);
	Assert(List != NULL);
	Assert(List->Type != NULL);
	Assert(List->Interpreter != NULL);
	sachi_size_t Size = SachiList_Size(List);
	Assert(Size == 0);
	Assert(SachiList_Empty(List));
	sachi_size_t Capacity = SachiList_Capacity(List);
	Assert(Capacity == INITIAL_CAPACITY);
	std::cout << "Initial list size=" << Size << std::endl;
	std::cout << "Initial list capacity=" << Capacity << std::endl;

	// Fill list
	Sachi_Object* Value = Sachi_NewStringFromBuffer(InInterpreter, "value");
	for (sachi_size_t I = 1; I <= MAX_CAPACITY; ++I)
	{
		SachiList_Push(List, Value);
		Size = SachiList_Size(List);
		Assert(Size == I);
		std::cout << "After push size=" << Size << std::endl;
	}

	Assert(!SachiList_Empty(List));

	// Set by index
	Assert(SachiList_SetItem(List, Size - 1, Value) == SACHI_OK);
	Assert(SachiList_SetItem(List, Size, Value) == SACHI_ERROR);

	// Get by index
	Sachi_Object* Item = NULL;
	Assert(SachiList_GetItem(List, Size - 1, &Item) == SACHI_OK);
	Assert(SachiList_GetItem(List, Size, &Item) == SACHI_ERROR);

	// Front and back
	Assert(SachiList_Front(List) != NULL);
	Assert(SachiList_Back(List) != NULL);

	// Empty list
	for (sachi_size_t I = MAX_CAPACITY; I >= 1; --I)
	{
		SachiList_Pop(List, NULL);
		Size = SachiList_Size(List);
		Assert(Size == (I - 1));
		std::cout << "After pop size=" << Size << std::endl;
	}

	Assert(SachiList_Empty(List));
	Assert(SachiList_Capacity(List) == MAX_CAPACITY);

	// List already empty
	SachiList_Pop(List, NULL);
	Size = SachiList_Size(List);
	Assert(Size == 0);
	std::cout << "After pop size=" << Size << std::endl;

	Sachi_DecRef(Value);
	Sachi_DecRef(List);
}

#endif