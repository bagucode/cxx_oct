#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Primitives.h"

namespace octarine {

  struct Object {
	Address object;
	Address vtable;
  };

}

#endif
