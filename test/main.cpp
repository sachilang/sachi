#include "test/test_int.h"
#include "test/test_string.h"
#include "test/test_list.h"
#include "test/test_dict.h"
#include "test/test_node.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/list.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

int main()
{
	Sachi_Interpreter* Interpreter = Sachi_NewInterpreter();
	assert(Interpreter != NULL);
	Sachi_InitType(&Sachi_BoolType);
	Sachi_InitType(&Sachi_IntType);
	Sachi_InitType(&Sachi_StringType);
	Sachi_InitType(&Sachi_ListType);
	Sachi_InitType(&Sachi_DictType);
	Sachi_InitType(&Sachi_NodeType);

	test_int(Interpreter);
	test_string(Interpreter);
	test_list(Interpreter);
	test_dict(Interpreter);
	test_node(Interpreter);
	return 0;
}
