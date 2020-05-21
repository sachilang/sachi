#include "sachi/compile.h"
#include "sachi/sachi.h"
#include "sachi/posix.h"
#include "sachi/object/node.h"

typedef unsigned char _Sachi_Opcode;
#define SACHI_OPCODE_UNKNOWN 0
#define SACHI_OPCODE_IF 1
#define SACHI_OPCODE_FORIN 2
#define SACHI_OPCODE_WHILE 3
#define SACHI_TOKEN_IF "if"
#define SACHI_TOKEN_FORIN "forin"
#define SACHI_TOKEN_WHILE "while"

/**
 * Mapping between a builtin token name to its opcode.
 */
typedef struct _Sachi_TokenOpcode
{
	const char* Token;
	_Sachi_Opcode Opcode;
} _Sachi_TokenOpcode;

/**
 * List all known builtin tokens.
 */
static _Sachi_TokenOpcode _Sachi_Opcodes[] = {
	{SACHI_TOKEN_IF, SACHI_OPCODE_IF},
	{SACHI_TOKEN_FORIN, SACHI_OPCODE_FORIN},
	{SACHI_TOKEN_WHILE, SACHI_OPCODE_WHILE},
	NULL
};

/**
 * Find an opcode based on its token.
 *
 * :param InToken: token
 * :return: found opcode
 */
static _Sachi_Opcode _Sachi_FindOpcode(const char* InToken)
{
	_Sachi_TokenOpcode* Opcode = &_Sachi_Opcodes;
	while (Opcode->Token)
	{
		if (sachi_strcmp(Opcode->Token, InToken) == 0)
		{
			return Opcode->Opcode;
		}
		Opcode++;
	}

	return SACHI_OPCODE_UNKNOWN;
}

typedef struct _Sachi_Program
{
	Sachi_Node* Node; // source node
} Sachi_Program;

SACHI_PUBLIC(int) Sachi_Compile(Sachi_Node* InNode, Sachi_Program** OutProgram)
{
	const char* Name = SachiNode_GetName(InNode);
	_Sachi_Opcode Opcode = _Sachi_FindOpcode(Name);
	switch (Opcode)
	{
	case SACHI_OPCODE_IF:
		break;
	case SACHI_OPCODE_FORIN:
		break;
	case SACHI_OPCODE_WHILE:
		break;
	case SACHI_OPCODE_UNKNOWN:
	default:
		return SACHI_ERROR;
	}
}