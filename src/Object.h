#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Protocol.h"

namespace octarine {

  struct Context;
  struct ObjectFunctions;

  typedef ProtocolObject<ObjectFunctions> Object;

  struct ObjectFunctions {
	Object (*copy)(Context* ctx, Object self);
	Object (*copyToHeap)(Context* ctx, Object self);
	Uword (*getSize)(Context* ctx, Object self);
	void (*trace)(Context* ctx, Object self);
  };

  inline Object copy(Context* ctx, Object o) {
	return o.vtable->functions.copy(ctx, o);
  }

  inline Object copyToHeap(Context* ctx, Object o) {
	return o.vtable->functions.copyToHeap(ctx, o);
  }

  inline Uword getSize(Context* ctx, Object o) {
	return o.vtable->functions.getSize(ctx, o);
  }

  inline void trace(Context* ctx, Object o) {
	o.vtable->functions.trace(ctx, o);
  }

}

#endif
