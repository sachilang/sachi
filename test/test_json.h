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
#include "sachi/object/nodeinstance.h"
#include "sachi/object/metadata/metadatanode.h"
#include "sachi/node/json.h"

#ifdef __cplusplus
}
#endif

static const char* JSON_STRING = "{\"key\": \"value\", \"list\": [\"value\", \"value\"]}";

void test_json_loads(Sachi_Interpreter* InInterpreter, Sachi_Object* InNode)
{
	Sachi_Object* LoadsNode = SachiNode_GetNode(InNode, "loads");
	Assert(LoadsNode != NULL);
	Sachi_Object* LoadsInstance = Sachi_NewNodeInstance(LoadsNode);
	Assert(LoadsInstance != NULL);

	Sachi_Object* KwArgs = Sachi_NewDict(InInterpreter);
	Sachi_Object* S = Sachi_NewStringFromBuffer(InInterpreter, JSON_STRING);
	Assert(SachiDict_SetItemFromBuffer(KwArgs, "s", S) == SACHI_OK);
	Sachi_DecRef(S);

	Sachi_Object* KwResults = NULL;
	Assert(SachiNodeInstance_CallWithArgs(LoadsInstance, NULL, NULL, KwArgs, NULL, &KwResults) == SACHI_OK);
	Assert(KwResults != NULL);

	Sachi_Object* Result = NULL;
	SachiDict_GetItemFromBuffer(KwResults, "o", &Result);

	Assert(SachiDict_Size(Result) == 2);
	std::cout << Result->Type->ToString(Result) << std::endl;

	Sachi_DecRef(LoadsInstance);
	Sachi_DecRef(KwArgs);
	Sachi_DecRef(KwResults);
}

void test_json_load(Sachi_Interpreter* InInterpreter, Sachi_Object* InNode)
{
	Sachi_Object* LoadNode = SachiNode_GetNode(InNode, "load");
	Assert(LoadNode != NULL);
	Sachi_Object* LoadInstance = Sachi_NewNodeInstance(LoadNode);
	Assert(LoadInstance != NULL);

	Sachi_Object* KwArgs = Sachi_NewDict(InInterpreter);
	Sachi_Object* S = Sachi_NewStringFromBuffer(InInterpreter, "test/test.json");
	Assert(SachiDict_SetItemFromBuffer(KwArgs, "s", S) == SACHI_OK);
	Sachi_DecRef(S);

	Sachi_Object* KwResults = NULL;
	Assert(SachiNodeInstance_CallWithArgs(LoadInstance, NULL, NULL, KwArgs, NULL, &KwResults) == SACHI_OK);
	Assert(KwResults != NULL);

	Sachi_Object* Result = NULL;
	SachiDict_GetItemFromBuffer(KwResults, "o", &Result);
	std::cout << Result->Type->ToString(Result) << std::endl;

	Sachi_Object* Node = Sachi_NewNodeFromDict(InInterpreter, Result);
	Assert(sachi_strcmp(SachiNode_GetName(Node), "math") == 0);

	Sachi_DecRef(Node);
	Sachi_DecRef(LoadInstance);
	Sachi_DecRef(KwArgs);
	Sachi_DecRef(KwResults);
}

void test_json(Sachi_Interpreter* InInterpreter)
{
	Sachi_Object* Node = Sachi_NewNodeFromMetadata(InInterpreter, &Sachi_JSONNodeMetadata);
	Assert(Node != NULL);

	test_json_loads(InInterpreter, Node);
	test_json_load(InInterpreter, Node);

	Sachi_DecRef(Node);
}

#endif