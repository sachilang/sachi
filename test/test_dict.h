#ifndef TEST_DICT_H
#define TEST_DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/bool.h"

#ifdef __cplusplus
}
#endif

void test_dict(Sachi_Interpreter* InInterpreter)
{
	// Create dict
	Sachi_Object* Dict = Sachi_DictType.New(InInterpreter);
	assert(Dict != NULL);
	assert(Dict->Type != NULL);
	assert(Dict->Interpreter != NULL);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	SachiDict_SetItem(Dict, Sachi_True, Sachi_True);
	assert(SachiDict_Size(Dict) == 1);
	assert(Sachi_IsFalse(SachiDict_Empty(Dict)));
	SachiDict_SetItem(Dict, Sachi_True, Sachi_False);
	assert(SachiDict_Size(Dict) == 1);
	SachiDict_SetItem(Dict, Sachi_False, Sachi_False);
	assert(SachiDict_Size(Dict) == 2);
	SachiDict_GetItem(Dict, Sachi_True);
	SachiDict_GetItem(Dict, Sachi_False);

	SachiDict_RemoveItem(Dict, Sachi_True);
	SachiDict_RemoveItem(Dict, Sachi_False);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	SachiDict_SetItem(Dict, Sachi_True, Sachi_True);
	SachiDict_SetItem(Dict, Sachi_False, Sachi_True);
	SachiDict_Clear(Dict);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	// Destroy
	Sachi_DictType.Delete(Dict);
}

#endif