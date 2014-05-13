#ifndef OCT_CONTEXT
#define OCT_CONTEXT

#include "Heap.h"

namespace octarine {

  struct Runtime;

  struct Context {
	Runtime* runtime;
	Heap heap;
  };

}

#endif
