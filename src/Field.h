#ifndef OCT_FIELD_H
#define OCT_FIELD_H

#include "Object.h"
#include "String.h"

namespace octarine {

	enum FieldKind {
		VALUE,
		POINTER
	};

	struct Type;

	struct Field {
		Type* _type;
		FieldKind _kind;
		Uword _offset;
		String _name;

		// Object functions
		void init();
		void destroy();
		Type* type();
		Uword hash();
		Bool equals(Type* other);
		void trace(MemoryManager mm);

		// Protocol casting functions
		Object asObject();
	};

	Type* FieldType;

}

#endif
