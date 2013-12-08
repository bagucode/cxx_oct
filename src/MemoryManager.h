#ifndef OCT_MEMORYMANAGER_H
#define OCT_MEMORYMANAGER_H

#include "Object.h"

namespace octarine {

	struct Self;
	struct MemoryManagerFunctions;
	struct Type;
	struct Object;

	struct MemoryManager {
		Self* mSelf;
		MemoryManagerFunctions* mFunctions;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

	enum MemoryManagerMarkResult {
		NOT_FOUND = 0,
		ALREADY_MARKED = 1,
		MARKED = 2
	};

	struct MemoryManagerFunctions {
		void(*alloc)(Self* self, Type t, Object* ret);
		void(*mark)(Self* self, Address location, Uword* ret);
	};

}

#endif
