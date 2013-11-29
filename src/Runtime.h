#ifndef OCT_RUNTIME_H
#define OCT_RUNTIME_H

#include "ThreadContext.h"
#include "Namespace.h"
#include "Function.h"
#include "String.h"

namespace octarine {

	struct RuntimeImpl;

	class Runtime {
	private:
		RuntimeImpl* _impl;
	public:
		Runtime();
		~Runtime();
		void registerFunction(ThreadContext tc, Namespace ns, Function f);
		ThreadContext getThreadContext();
	};

}

#endif
