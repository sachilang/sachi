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

static Sachi_PinDef PinDefs[] = {
	{NULL, "exechello", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execworld", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	NULL
};

static int _HelloWorld(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	if (sachi_strcmp(SachiPin_GetName(InInputExecPin), "exechello") == 0)
	{
		std::cout << "Hello" << std::endl;
	}
	else if (sachi_strcmp(SachiPin_GetName(InInputExecPin), "execworld") == 0)
	{
		std::cout << " World !" << std::endl;
	}

	return SACHI_OK;
}

static Sachi_NodeDef NodeDef = {
	"helloworld",
	_HelloWorld,
	PinDefs
};

void test_node()
{

	Sachi_Interpreter* Interpreter = Sachi_NewInterpreter();
	assert(Interpreter != NULL);

	// Create node
	Sachi_Object* Node = Sachi_NodeType.New(Interpreter);
	assert(Node != NULL);

	assert(SachiNode_SetDefition(Node, &NodeDef) == SACHI_OK);

	Sachi_NodeDef* OutDefinition = SachiNode_GetDefition(Node);
	assert(OutDefinition != NULL);

	Sachi_Object* Pins = SachiNode_GetPins(Node);
	assert(Pins != NULL);

	OutDefinition->Func(
		Interpreter,
		NULL,
		SachiList_GetItem(Pins, 0),
		NULL,
		NULL,
		NULL
	);

	OutDefinition->Func(
		Interpreter,
		NULL,
		SachiList_GetItem(Pins, 1),
		NULL,
		NULL,
		NULL
	);

	// Destroy
	Sachi_NodeType.Delete(Node);

	Sachi_DeleteInterpreter(Interpreter);
}

#endif