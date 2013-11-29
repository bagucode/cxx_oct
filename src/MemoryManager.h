#ifndef OCT_MEMORYMANAGER_H
#define OCT_MEMORYMANAGER_H

#include "Primitives.h"
#include "Type.h"
#include "ThreadContext.h"
#include "Object.h"

namespace octarine {

	struct MemoryManagerT;

	typedef MemoryManagerT MemoryManager;

	Object alloc(ThreadContext tc, MemoryManager mm, Type t);

}

#endif

