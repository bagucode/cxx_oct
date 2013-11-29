#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Type.h"

namespace octarine {

	struct ObjectT;

	typedef ObjectT* Object;

	Type typeOf(Object obj);

}

#endif
