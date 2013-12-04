#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Object.h"

#include <string>

namespace octarine {

	struct String {
		const char* mStr; // TODO: Proper string. Probably use OS string types, they have unicode support.

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
