#include "Context.h"

namespace octarine {

  struct Context {
	Runtime* runtime;
	Heap heap;
  };

  Heap getHeap(Context* ctx, Context* heapCtx) {
	return heapCtx->heap;
  }

  Runtime* getRuntime(Context* ctx) {
	return ctx->runtime;
  }

}
