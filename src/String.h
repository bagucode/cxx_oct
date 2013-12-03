#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Object.h"

#include <string>

namespace octarine {

	struct String {
		const char* str; // TODO: Proper string. Probably use OS string types, they have unicode support.

		static Type* type;
		static ObjectFunctions objectFns;
		Object asObject();
	};

	Type* StringType;

}

#endif
