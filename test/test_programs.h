#ifndef TEST_PROGRAMS_H
#define TEST_PROGRAMS_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/compile.h"
#include "sachi/nodevisitor.h"
#include "sachi/node/json.h"
#include "sachi/object/interpreter.h"
#include "sachi/object/int.h"
#include "sachi/object/bool.h"
#include "sachi/object/node.h"
#include "sachi/object/nodeinstance.h"
#include "sachi/object/json/jsonnode.h"

#ifdef __cplusplus
}
#endif

void visit_node(Sachi_Object* InNode)
{
	std::cout << "{\"name\": \"" << SachiNode_GetName(InNode) << "\"}" << std::endl;
}

void visit_pin(Sachi_Object* InPin)
{
	std::cout << "- {" << \
		"name\": \"" << SachiPin_GetName(InPin) << "\", " << \
		"mode\": \"" << (int)SachiPin_GetMode(InPin) << "\", " << \
		"side\": \"" << (int)SachiPin_GetSide(InPin) << "\"" << \
	"}" << std::endl;
}

void visit_flow(Sachi_Object* InPin)
{
	std::cout << "- \"flow\":" << std::endl;
}

void visit_node_instance(Sachi_Object* InNodeInstance)
{
	std::cout << SachiNodeInstance_Get << std::endl;
}

Sachi_NodeVisitor Visitor = {
	visit_node,
	visit_pin,
	visit_flow,
	visit_node_instance
};

void run_program(Sachi_Interpreter* InInterpreter, const char* InName)
{
	Sachi_Object* Dict = NULL;
	Assert(SachiJSON_LoadWithBuffer(InInterpreter, InName, &Dict) == SACHI_OK);
	Assert(Dict != NULL);

	Sachi_Object* Node = Sachi_NewNodeFromJSON(InInterpreter, Dict);
	Sachi_DecRef(Dict);
	if (!Node && SachiInterpreter_GetErrorMessage((Sachi_Object*)InInterpreter))
	{
		std::cout << SachiInterpreter_GetErrorMessage((Sachi_Object*)InInterpreter) << std::endl;
	}
	Assert(Node != NULL);

	Sachi_VisitNode(Node, &Visitor);

	Sachi_Program* Program;
	Assert(Sachi_Compile((Sachi_Node*)Node, &Program) == SACHI_OK);
	Sachi_DecRef(Node);
}

void test_programs(Sachi_Interpreter* InInterpreter)
{
	run_program(InInterpreter, "test/programs/test_primitives.json");
}

#endif