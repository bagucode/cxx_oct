#ifndef OCT_CONTEXT
#define OCT_CONTEXT

#include "Heap.h"

namespace octarine {

  struct Runtime;
  struct Context;

  Runtime* getRuntime(Context* ctx);
  Heap getHeap(Context* ctx, Context* heapCtx);

}

#endif
