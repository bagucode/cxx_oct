#ifndef OCT_HEAPFUNCTIONS_H
#define OCT_HEAPFUNCTIONS_H

#include "Heap.h"
#include "Object.h"

namespace octarine {

  struct Context;
  struct Type;

  struct HeapFunctions {
	Object (*alloc)(Context* ctx, Heap heap, Type* type, Object constructArg);
	Address (*rawAlloc)(Context* ctx, Heap heap, Uword size);
	void (*setType)(Context* ctx, Heap heap, Address location, Type* type);
  };

  struct HeapVTable {
	Type* type;
	HeapFunctions functions;
  };

  inline Object alloc(Context* ctx, Heap heap, Type* type, Object constructArg) {
	return ((HeapVTable*)heap.vtable)->functions.alloc(ctx, heap, type, constructArg);
  }

  inline Address rawAlloc(Context* ctx, Heap heap, Uword size) {
	return ((HeapVTable*)heap.vtable)->functions.rawAlloc(ctx, heap, size);
  }

  inline void setType(Context* ctx, Heap heap, Address location, Type* type) {
	return ((HeapVTable*)heap.vtable)->functions.setType(ctx, heap, location, type);
  }

}

#endif

