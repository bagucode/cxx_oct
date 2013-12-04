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
		Type* mType;
		Uword mKind;
		Uword mOffset;
		String mName;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
