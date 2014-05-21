#include "LeakHeap.h"
#include "Type.h"

namespace octarine {

  struct LeakHeap {
	Uword size; // or something...
  };

  Object alloc(Context* ctx, LeakHeap* heap, Type* type) {
	Uword size = getSize(ctx, type);
	
  }

  Address rawAlloc(Context* ctx, LeakHeap* heap, Uword size) {
  }

  void setType(Context* ctx, LeakHeap* heap, Address location, Type* type) {
  }

}
