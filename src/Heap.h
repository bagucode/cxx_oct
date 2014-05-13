#ifndef OCT_HEAP_H
#define OCT_HEAP_H

#include "Primitives.h"

namespace octarine {

  struct Heap {
	Address object;
	Address vtable;
  };

}

#endif
