#include "Runtime.h"
#include "Primitives.h"

namespace octarine {

	struct RuntimeT {
#ifdef WINDOWS
		DWORD threadLocalStorage;
#elif MACOSX
#else
#endif

	};

	void registerFunction(ThreadContext tc, Runtime rt, Namespace ns, Function f) {
	}

	ThreadContext getThreadContext(Runtime rt) {

	}

	Runtime createRuntime() {

	}

}

