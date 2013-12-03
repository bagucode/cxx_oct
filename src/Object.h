#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Primitives.h"
#include "MemoryManager.h"

namespace octarine {

	struct Self;
	struct ObjectFunctions;
	struct MemoryManager;

	struct Object {
		Self* self;
		ObjectFunctions* functions;
	};

	struct ObjectFunctions {
		void(*init)(Self* self);
		void(*destroy)(Self* self);
		Type*(*type)(Self* self);
		Uword(*hash)(Self* self);
		Bool(*equals)(Self* self, Object other);
		void(*trace)(Self* self, MemoryManager mm);
		// some function for getting metadata?
	};

	Type* ObjectType;

}

#endif
