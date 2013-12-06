#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Primitives.h"

namespace octarine {

	struct Self;
	struct ObjectFunctions;

	struct Object {
		Self* mSelf;
		ObjectFunctions* mFunctions;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

	struct ObjectFunctions {
		void(*init)(Self* self);
		void(*destroy)(Self* self);
		Type*(*type)(Self* self);
		Uword(*hash)(Self* self);
		Bool(*equals)(Self* self, Object other);
		void(*trace)(Self* self, MemoryManager mm);
		// some function for getting metadata?

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
