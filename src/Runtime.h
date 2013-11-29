#ifndef OCT_RUNTIME_H
#define OCT_RUNTIME_H

#include "ThreadContext.h"
#include "Namespace.h"
#include "Function.h"
#include "String.h"

namespace octarine {

	struct RuntimeT;

	typedef RuntimeT* Runtime;

	void registerFunction(ThreadContext tc, Runtime rt, Namespace ns, Function f);
	ThreadContext getThreadContext(Runtime rt);
	Runtime createRuntime();
}

#endif
