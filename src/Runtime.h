#ifndef OCT_RUNTIME
#define OCT_RUNTIME

#include "Heap.h"

namespace octarine {

  struct Context;
  struct Runtime;

  Heap getHeap(Context* ctx, Runtime* rt);

}

#endif
