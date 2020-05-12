#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/parser.h"
#include "sachi/module.h"

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

	Sachi_Node* Node = SachiParser_Load("test/test.json");
	if (!Node)
	{
		std::cout << SachiParser_GetErrorPtr() << std::endl;
		return 1;
	}

	std::cout << "Loaded node:" << std::endl;
	std::cout << "- name: " << Node->Name << std::endl;
	std::cout << "- inputs:";
	if (Node->NumInputs > 0)
	{
		std::cout << std::endl;
		for (sachi_size_t I = 0; I < Node->NumInputs; ++I)
		{
			std::cout << "  - name: " << Node->Inputs[I].Name << std::endl;
		}
	}
	std::cout << "- outputs:";
	if (Node->NumOutputs > 0)
	{
		std::cout << std::endl;
		for (sachi_size_t I = 0; I < Node->NumOutputs; ++I)
		{
			std::cout << "  - name: " << Node->Outputs[I].Name << std::endl;
		}
	}

	Sachi_DeleteNode(Node);
	return 0;
}