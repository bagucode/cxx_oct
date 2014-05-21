#ifndef OCT_LEAKHEAP_H
#define OCT_LEAKHEAP_H

#include "HeapFunctions.h"

namespace octarine {

  struct LeakHeap;

  Object alloc(Context* ctx, LeakHeap* heap, Type* type);
  Address rawAlloc(Context* ctx, LeakHeap* heap, Uword size);
  void setType(Context* ctx, LeakHeap* heap, Address location, Type* type);

}

#endif

