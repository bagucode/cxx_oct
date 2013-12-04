#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Object.h"

namespace octarine {

	struct Array {
		Type* _type;
		Uword _size;
		Address _data;

		static Type* type;
		static ObjectFunctions* objectFns;
		Object asObject();
	};

}

#endif
