#ifndef TEST_NODE_H
#define TEST_NODE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/node.h"
#include "sachi/object/metadata/metadatanode.h"
#include "sachi/object/nodeinstance.h"
#include "sachi/object/pin.h"
#include "sachi/object/list.h"
#include "sachi/object/dict.h"
#include "sachi/object/int.h"
#include "sachi/object/string.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"

#ifdef __cplusplus
}
#endif

#define _SACHI_HELLOWORLD_EXECHELLO 0
#define _SACHI_HELLOWORLD_EXECWORLD 1
#define _SACHI_HELLOWORLD_EXECOUT 2

static Sachi_PinMetadata _HelloWorldPins[] = {
	{"exechello", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execworld", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	NULL
};

static int _HelloWorld(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	if (sachi_strcmp(SachiPin_GetName(InInputExecPin), _HelloWorldPins[_SACHI_HELLOWORLD_EXECHELLO].Name) == 0)
	{
		std::cout << "Hello" << std::endl;
	}
	else if (sachi_strcmp(SachiPin_GetName(InInputExecPin), _HelloWorldPins[_SACHI_HELLOWORLD_EXECWORLD].Name) == 0)
	{
		std::cout << " World !" << std::endl;
	}

	if (OutOutputExecPin)
	{
		*OutOutputExecPin = SachiNode_GetPin(
			SachiNodeInstance_GetNode(InNodeInstance),
			_HelloWorldPins[_SACHI_HELLOWORLD_EXECOUT].Name
		);
	}

	return SACHI_OK;
}

#define _SACHI_MIN_A 0
#define _SACHI_MIN_B 1
#define _SACHI_MIN_OUT 2

static Sachi_PinMetadata _MinPins[] = {
	{"a", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"b", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"out", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT},
	NULL
};

static int _Min(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	Sachi_Object* A = NULL;
	Sachi_Object* B = NULL;
	Assert(SachiDict_GetItemFromBuffer(InKwArgs, "a", &A) == SACHI_OK);
	Assert(SachiDict_GetItemFromBuffer(InKwArgs, "b", &B) == SACHI_OK);
	Sachi_Object* Out = SachiInt_Data(A) <= SachiInt_Data(B) ? A : B;
	SachiDict_SetItemFromBuffer(InKwResults, "out", Out);

	std::cout << "Min a=" << SachiInt_Data(A) << ", b=" << SachiInt_Data(B) << ", out=" << SachiInt_Data(Out) << std::endl;
	return SACHI_OK;
}

static Sachi_NodeMetadata _Methods[] = {
	{"helloworld", _HelloWorld, _HelloWorldPins},
	{"min", _Min, _MinPins},
	NULL
};

static Sachi_NodeMetadata _Node = {
	"package",
	NULL,
	NULL,
	_Methods
};

void test_node(Sachi_Interpreter* InInterpreter)
{
	// Create the Package node
	Sachi_Object* Node = Sachi_NewNodeFromMetadata(InInterpreter, &_Node);
	Assert(Node != NULL);
	Assert(Node->Type != NULL);
	Assert(Node->Interpreter != NULL);

	Sachi_Object* HelloWorldNode = SachiNode_GetNode(Node, "helloworld");
	Assert(HelloWorldNode != NULL);
	Assert(SachiList_Size(SachiNode_GetPins(HelloWorldNode)) == 3);
	Sachi_Object* HelloWorldInstance = Sachi_NewNodeInstance(HelloWorldNode);
	Assert(HelloWorldInstance != NULL);

	Sachi_Object* MinNode = SachiNode_GetNode(Node, "min");
	Assert(MinNode != NULL);
	Assert(SachiList_Size(SachiNode_GetPins(MinNode)) == 3);
	Sachi_Object* MinInstance = Sachi_NewNodeInstance(MinNode);
	Assert(MinInstance != NULL);

	// Test HelloWorld
	Sachi_Object* Pins = SachiNode_GetPins(HelloWorldNode);
	Assert(Pins != NULL);

	Sachi_Object* ExecPin = NULL;
	Assert(SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECHELLO, &ExecPin) == SACHI_OK);
	Assert(SachiNodeInstance_CallWithArgs(HelloWorldInstance, NULL, ExecPin, NULL, NULL, NULL) == SACHI_OK);
	Assert(SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECWORLD, &ExecPin) == SACHI_OK);
	Assert(SachiNodeInstance_CallWithArgs(HelloWorldInstance, NULL, ExecPin, NULL, NULL, NULL) == SACHI_OK);

	// Test Min
	Pins = SachiNode_GetPins(MinNode);
	Assert(Pins != NULL);

	Sachi_Object* KwArgs = Sachi_NewDict(InInterpreter);
	Assert(KwArgs != NULL);

	Sachi_Object* A = Sachi_NewIntFromValue(InInterpreter, 1);
	Sachi_Object* B = Sachi_NewIntFromValue(InInterpreter, 2);
	SachiDict_SetItemFromBuffer(KwArgs, "a", A);
	SachiDict_SetItemFromBuffer(KwArgs, "b", B);

	Sachi_Object* KwResults = NULL;
	Assert(SachiNodeInstance_CallWithArgs(MinInstance, NULL, ExecPin, KwArgs, NULL, &KwResults) == SACHI_OK);

	Sachi_DecRef(MinInstance);
	Sachi_DecRef(HelloWorldInstance);
	Sachi_DecRef(A);
	Sachi_DecRef(B);
	Sachi_DecRef(KwArgs);
	Sachi_DecRef(KwResults);
	Sachi_DecRef(Node);
}

#endif