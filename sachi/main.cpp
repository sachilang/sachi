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

	Sachi_Interpreter* Interpreter = SachiInterpreter_NewInterpreter();
	if (SachiInterpreter_RunNode(Interpreter, Node) != SACHI_OK)
	{
		std::cout << SachiInterpreter_GetErrorPtr() << std::endl;
		return 1;
	}

	Sachi_DeleteNode(Node);
	return 0;
}