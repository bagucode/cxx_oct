#ifndef OCT_MEMORYMANAGER_H
#define OCT_MEMORYMANAGER_H

#include "Primitives.h"
#include "Type.h"
#include "ThreadContext.h"
#include "Object.h"

namespace octarine {

	struct MemoryManagerT;
	typedef MemoryManagerT MemoryManager;

	struct MemoryManagerProtocol {
		Object(*rawAlloc)(ThreadContext tc, MemoryManager mm, Type t);
		Object(*allocAndInit)(ThreadContext tc, MemoryManager mm, Type t);
	};

}

#endif

