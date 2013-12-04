#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Array.h"

namespace octarine {

	struct Type {
		Uword mSize;
		Uword mAlignment;
		Array mFields;
		ObjectFunctions* mObjectFns;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
