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
#include "sachi/object/pin.h"
#include "sachi/object/list.h"
#include "sachi/object/dict.h"
#include "sachi/object/int.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

#define _SACHI_HELLOWORLD_EXECHELLO 0
#define _SACHI_HELLOWORLD_EXECWORLD 1
#define _SACHI_HELLOWORLD_EXECOUT 2

static Sachi_PinDef HelloWorldPinsDefs[] = {
	{NULL, "exechello", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execworld", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT, NULL, NULL},
	NULL
};

static int _HelloWorld(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	if (sachi_strcmp(SachiPin_GetName(InInputExecPin), HelloWorldPinsDefs[_SACHI_HELLOWORLD_EXECHELLO].Name) == 0)
	{
		std::cout << "Hello" << std::endl;
	}
	else if (sachi_strcmp(SachiPin_GetName(InInputExecPin), HelloWorldPinsDefs[_SACHI_HELLOWORLD_EXECWORLD].Name) == 0)
	{
		std::cout << " World !" << std::endl;
	}

	if (OutOutputExecPin)
	{
		*OutOutputExecPin = SachiNode_GetPin(InObject, HelloWorldPinsDefs[_SACHI_HELLOWORLD_EXECOUT].Name);
	}

	return SACHI_OK;
}

#define _SACHI_MIN_A 0
#define _SACHI_MIN_B 1
#define _SACHI_MIN_OUT 2

static Sachi_PinDef MinPinsDefs[] = {
	{NULL, "a", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "b", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "out", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT, NULL, NULL},
	NULL
};

static int _Min(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	Sachi_Object* A = NULL;
	Sachi_Object* B = NULL;
	Assert(SachiDict_GetItemFromBuffer(InKwArgs, "a", &A) == SACHI_OK);
	Assert(SachiDict_GetItemFromBuffer(InKwArgs, "b", &B) == SACHI_OK);
	Sachi_Object* Out = SachiInt_Data(A) <= SachiInt_Data(B) ? A : B;
	SachiDict_SetItemFromBuffer(OutKwResults, "out", Out);

	std::cout << "Min a=" << SachiInt_Data(A) << ", b=" << SachiInt_Data(B) << ", out=" << SachiInt_Data(Out) << std::endl;
	return SACHI_OK;
}

static Sachi_NodeDef PackageChildrenDefs[] = {
	{"helloworld", _HelloWorld, HelloWorldPinsDefs},
	{"min", _Min, MinPinsDefs},
	NULL
};

static Sachi_NodeDef PackageDef = {
	"package",
	NULL,
	NULL,
	PackageChildrenDefs
};

void test_node(Sachi_Interpreter* InInterpreter)
{
	// Create the Package node
	Sachi_Object* Node = Sachi_NodeType.New(InInterpreter);
	Assert(Node != NULL);
	Assert(Node->Type != NULL);
	Assert(Node->Interpreter != NULL);

	Assert(SachiNode_SetDefition(Node, &PackageDef) == SACHI_OK);

	Sachi_Object* HelloWorldNode = SachiNode_GetChild(Node, "helloworld");
	Assert(HelloWorldNode != NULL);

	Sachi_Object* MinNode = SachiNode_GetChild(Node, "min");
	Assert(MinNode != NULL);

	// Test HelloWorld
	Sachi_NodeDef* OutDefinition = SachiNode_GetDefition(HelloWorldNode);
	Assert(OutDefinition != NULL);

	Sachi_Object* Pins = SachiNode_GetPins(HelloWorldNode);
	Assert(Pins != NULL);

	Sachi_Object* ExecPin = NULL;
	Assert(SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECHELLO, &ExecPin) == SACHI_OK);

	OutDefinition->Func(
		InInterpreter,
		HelloWorldNode,
		ExecPin,
		NULL,
		NULL,
		NULL
	);

	Assert(SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECWORLD, &ExecPin) == SACHI_OK);

	OutDefinition->Func(
		InInterpreter,
		HelloWorldNode,
		ExecPin,
		NULL,
		NULL,
		NULL
	);

	// Test Min
	OutDefinition = SachiNode_GetDefition(MinNode);
	Assert(OutDefinition != NULL);

	Pins = SachiNode_GetPins(MinNode);
	Assert(Pins != NULL);

	Sachi_Object* KwArgs = Sachi_NewDict(InInterpreter);
	Assert(KwArgs != NULL);

	Sachi_Object* A = Sachi_NewIntFromValue(InInterpreter, 1);
	Sachi_Object* B = Sachi_NewIntFromValue(InInterpreter, 2);
	SachiDict_SetItemFromBuffer(KwArgs, "a", A);
	SachiDict_SetItemFromBuffer(KwArgs, "b", B);

	Sachi_Object* KwResults = Sachi_NewDict(InInterpreter);
	Assert(KwResults != NULL);

	OutDefinition->Func(
		InInterpreter,
		MinNode,
		NULL,
		KwArgs,
		NULL,
		KwResults
	);

	Sachi_DecRef(A);
	Sachi_DecRef(B);
	Sachi_DecRef(KwArgs);
	Sachi_DecRef(KwResults);
	Sachi_DecRef(Node);
}

#endif