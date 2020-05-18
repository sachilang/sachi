#ifndef TEST_JSON_H
#define TEST_JSON_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/node/json.h"

#ifdef __cplusplus
}
#endif

static const char* JSON_STRING = "{\"key\": \"value\"}";

void test_json(Sachi_Interpreter* InInterpreter)
{
	Sachi_Object* Node = Sachi_NewJSON(InInterpreter);
	Assert(Node != NULL);

	Sachi_Object* Loads = SachiNode_GetChild(Node, "loads");
	Assert(Loads != NULL);

	Sachi_Object* KwArgs = Sachi_NewDict(InInterpreter);
	Sachi_Object* S = Sachi_NewStringFromBuffer(InInterpreter, JSON_STRING);
	Assert(SachiDict_SetItemFromBuffer(KwArgs, "s", S) == SACHI_OK);
	Sachi_DecRef(S);

	Assert(SachiNode_Call(Loads, NULL, KwArgs, NULL, NULL) == SACHI_OK);

	Sachi_DecRef(KwArgs);
	Sachi_DecRef(Node);
}

#endif