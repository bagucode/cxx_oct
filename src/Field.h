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
		Uword _kind;
		Uword _offset;
		String _name;

		static Type* type;
		static ObjectFunctions objectFns;
		Object asObject();
	};

}

#endif
