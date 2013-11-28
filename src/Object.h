#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Type.h"

namespace octarine {

	struct ObjectT;
	typedef ObjectT* Object;

	struct ObjectProtocol {
		Type(*getType)(Object obj);
	};

}

#endif
