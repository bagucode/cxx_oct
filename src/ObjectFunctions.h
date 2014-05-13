#ifndef OCT_OBJECTFUNCTIONS_H
#define OCT_OBJECTFUNCTIONS_H

#include "Object.h"
#include "Heap.h"

namespace octarine {

  struct Context;
  struct Type;

  struct ObjectFunctions {
	Object (*copy)(Context* ctx, Object self);
	Object (*copyToHeap)(Context* ctx, Heap heap, Object self);
	Uword (*getSize)(Context* ctx, Object self);
	void (*trace)(Context* ctx, Object self);
  };

  struct ObjectVTable {
	Type* type;
	ObjectFunctions functions;
  };

  inline Object copy(Context* ctx, Object o) {
	return ((ObjectVTable*)o.vtable)->functions.copy(ctx, o);
  }

  inline Object copyToHeap(Context* ctx, Heap heap, Object o) {
	return ((ObjectVTable*)o.vtable)->functions.copyToHeap(ctx, heap, o);
  }

  inline Uword getSize(Context* ctx, Object o) {
	return ((ObjectVTable*)o.vtable)->functions.getSize(ctx, o);
  }

  inline void trace(Context* ctx, Object o) {
	((ObjectVTable*)o.vtable)->functions.trace(ctx, o);
  }

}

#endif
