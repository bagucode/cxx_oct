#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Object.h"

namespace octarine {

	struct String {
		Uword mSize;
		const char* mData; // TODO: Proper string. Probably use OS string types, they have unicode support.

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
