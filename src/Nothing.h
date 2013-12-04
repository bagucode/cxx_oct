#ifndef OCT_NOTHING_H
#define OCT_NOTHING_H

#include "Object.h"

namespace octarine {

	struct Nothing {
		Uword mDummyValue; // Can't have zero size struct

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
