#ifndef TEST_NODE_H
#define TEST_NODE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/node.h"
#include "sachi/object/pin.h"

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

static int _HelloWorld(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
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

static int _Min(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	std::cout << "Min" << std::endl;
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

void test_node()
{

	Sachi_Interpreter* Interpreter = Sachi_NewInterpreter();
	assert(Interpreter != NULL);
	
	// Create the Package node
	Sachi_Object* Node = Sachi_NodeType.New(Interpreter);
	assert(Node != NULL);

	assert(SachiNode_SetDefition(Node, &PackageDef) == SACHI_OK);

	Sachi_Object* HelloWorldNode = SachiNode_GetChild(Node, "helloworld");
	assert(HelloWorldNode != NULL);

	Sachi_Object* MinNode = SachiNode_GetChild(Node, "min");
	assert(MinNode != NULL);

	// Test HelloWorld
	Sachi_NodeDef* OutDefinition = SachiNode_GetDefition(HelloWorldNode);
	assert(OutDefinition != NULL);

	Sachi_Object* Pins = SachiNode_GetPins(HelloWorldNode);
	assert(Pins != NULL);

	OutDefinition->Func(
		Interpreter,
		HelloWorldNode,
		SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECHELLO),
		NULL,
		NULL,
		NULL
	);

	OutDefinition->Func(
		Interpreter,
		HelloWorldNode,
		SachiList_GetItem(Pins, _SACHI_HELLOWORLD_EXECWORLD),
		NULL,
		NULL,
		NULL
	);

	// Test Min
	OutDefinition = SachiNode_GetDefition(MinNode);
	assert(OutDefinition != NULL);

	Pins = SachiNode_GetPins(MinNode);
	assert(Pins != NULL);

	OutDefinition->Func(
		Interpreter,
		MinNode,
		NULL,
		NULL,
		NULL,
		NULL
	);

	// Destroy
	Sachi_NodeType.Delete(Node);

	Sachi_DeleteInterpreter(Interpreter);
}

#endif