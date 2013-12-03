#ifndef OCT_EXCEPTION_H
#define OCT_EXCEPTION_H

#include "String.h"
#include "Object.h"

namespace octarine {

	struct Exception {
		String message;

		Object asObject();
	};

}

#endif

