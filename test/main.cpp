#include "test/test_case.h"
#include "test/test_int.h"
#include "test/test_string.h"
#include "test/test_list.h"
#include "test/test_dict.h"
#include "test/test_node.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/list.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

void run(void(*test)(Sachi_Interpreter*))
{
	Sachi_Interpreter* Interpreter = (Sachi_Interpreter*)Sachi_NewInterpreter(NULL);
	assert(Interpreter != NULL);
	test(Interpreter);
	Sachi_DeleteInterpreter((Sachi_Object*)Interpreter);
	assert(MemoryTracker.Next == NULL);
}

int main()
{
	Sachi_InitType(&Sachi_BoolType);
	Sachi_InitType(&Sachi_IntType);
	Sachi_InitType(&Sachi_StringType);
	Sachi_InitType(&Sachi_ListType);
	Sachi_InitType(&Sachi_DictType);
	Sachi_InitType(&Sachi_NodeType);

	run(test_int);
	run(test_string);
	run(test_list);
	run(test_dict);
	run(test_node);
	return 0;
}
