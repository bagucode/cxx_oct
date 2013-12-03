#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Object.h"

#include <string>

namespace octarine {

	struct String {
		std::string str; // TODO: Proper string. Probably use OS string types, they have unicode support.

		// To support conversion from string literals in C++ code
		String(const char* cstr);

		// Object functions
		void init();
		void destroy();
		Type* type();
		Uword hash();
		Bool equals(Type* other);
		void trace(MemoryManager* mm);

		// Protocol casting functions
		Object asObject();
	};

	Type* StringType;

}

#endif
