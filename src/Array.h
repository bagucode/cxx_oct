#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Object.h"

namespace octarine {

	struct Array {
		Type* mType;
		Uword mKind; // FieldKind
		Uword mSize;
		Address mData;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
