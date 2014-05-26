#include "LeakHeap.h"
#include "Type.h"
#include "ObjectFunctions.h"
#include <stdlib.h>

namespace octarine {

  struct LeakHeap {
	Uword size; // or something...
  };

  Object alloc(Context* ctx, LeakHeap* heap, Type* type) {
	Uword size = getSize(ctx, type);
	Object o;
	o.object = malloc(size);
	((ObjectVTable*)o.vtable)->type = type;
	// TODO: get the object protocol here and look up the Object protocol
	// implementation for the given type.
	ObjectFunctions* fns = &(((ObjectVTable*)o.vtable)->functions);
	fns->copy = 0;
	fns->copyToHeap = 0;
	fns->getSize = 0;
	fns->trace = 0;
	heap->size += size;
	return o;
  }

  Address rawAlloc(Context* ctx, LeakHeap* heap, Uword size) {
  }

  void setType(Context* ctx, LeakHeap* heap, Address location, Type* type) {
  }

}
