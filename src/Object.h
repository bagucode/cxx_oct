#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Protocol.h"

namespace octarine {

  struct ObjectFunctions;

  typedef ProtocolObject<ObjectFunctions> Object;

  struct ObjectFunctions {
	Object (*copy)();
	Object (*copyToHeap)();
	Uword (*getSize)();
	void (*trace)();
  };

  Object copy(Object o) {
	return o.vtable->functions.copy();
  }

  Object copyToHeap(Object o) {
	return o.vtable->functions.copyToHeap();
  }

  Uword getSize(Object o) {
	return o.vtable->functions.getSize();
  }

  void trace(Object o) {
	o.vtable->functions.trace();
  }

}

#endif
