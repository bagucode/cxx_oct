#include "Runtime.h"

namespace octarine {

  struct Runtime {
	Heap heap; // Could be more specific here, use GlobalHeap or whatever we call it
  };

  Heap getHeap(Context* ctx, Runtime* rt) {
	return rt->heap;
  }

}
