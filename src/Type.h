#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Array.h"

namespace octarine {

	struct Type {
		Uword _size;
		Uword _alignment;
		Array _fields;

		static Type* type;
		static ObjectFunctions objectFns;
		Object asObject();
	};

}

#endif
