#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Object.h"

namespace octarine {

	struct Type;

	struct Array {
		Type* _type;
		Uword _size;
		Address _data;

		// Object functions
		void init();
		void destroy();
		Type* type();
		Uword hash();
		Bool equals(Array* other);
		void trace(MemoryManager mm);

		// Protocol casting functions
		Object asObject();
	};

	Type* ArrayType;

}

#endif
