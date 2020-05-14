#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/sachi.h"
#include "sachi/parser.h"
#include "sachi/node.h"
#include "sachi/interpreter.h"

#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <iostream>

Sachi_Instance* NewInstance()
{
	Sachi_Instance* Instance = Sachi_NewInstance();
	if (!Instance)
	{
		const char* Msg = Sachi_GetErrorPtr();
		if (Msg)
		{
			std::cout << Msg << std::endl;
		}
		return NULL;
	}

	return Instance;
}

int main()
{
	std::cout << "Sachi running" << std::endl;

	Sachi_Node* Node = Sachi_Load("test/test.json");
	if (!Node)
	{
		const char* Msg = Sachi_GetErrorPtr();
		if (Msg)
		{
			std::cout << Msg << std::endl;
		}
		return 1;
	}

	std::cout << "Loaded node:" << std::endl;
	Sachi_PrintNode(Node);

	Sachi_Instance Instance;
	Instance.Path = "math";
	Instance.Name = "math";
	Sachi_Link Links[] = {
		{{"this", "execin"}, {"math", "execin"}},
		{{"math", "execout"}, {"this", "execout"}}
	};
	Sachi_Flow Flow;
	Flow.Instances = &Instance;
	Flow.NumInstances = 1;
	Flow.Links = &Links[0];
	Flow.NumLinks = 2;

	Sachi_Interpreter* Interpreter = Sachi_NewInterpreter();
	if (SachiInterpreter_LoadNode(Interpreter, Node) != SACHI_OK)
	{
		std::cout << Sachi_GetErrorPtr() << std::endl;
		return 1;
	}

	if (Sachi_RunFlow(Interpreter, &Flow) != SACHI_OK)
	{
		std::cout << Sachi_GetErrorPtr() << std::endl;
		return 1;
	}

	Sachi_DeleteNode(Node);
	return 0;
}