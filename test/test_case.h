#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/sachi.h"
#include "sachi/object/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

void Assert(int InCondition)
{
	if (!InCondition)
	{
		Sachi_MemoryTracker* Tracker = &MemoryTracker;
		assert(InCondition);
	}
}

#endif