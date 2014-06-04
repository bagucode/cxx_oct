#include "Runtime.h"
#include "LeakHeap.h"

namespace octarine {

  struct Runtime {
	Heap heap; // Could be more specific here, use GlobalHeap or whatever we call it
  };

  Runtime* createRuntime() {

  }

  Heap getHeap(Context* ctx, Runtime* rt) {
	return rt->heap;
  }

}
