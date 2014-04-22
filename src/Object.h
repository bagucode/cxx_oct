#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Protocol.h"

namespace octarine {

  struct ObjectFunctions;

  typedef ProtocolObject<ObjectFunctions> Object;

  struct ObjectFunctions {
	Object (*copy)(Object self);
	Object (*copyToHeap)(Object self);
	Uword (*getSize)(Object self);
	void (*trace)(Object self);
  };

  Object copy(Object o) {
	return o.vtable->functions.copy(o);
  }

  Object copyToHeap(Object o) {
	return o.vtable->functions.copyToHeap(o);
  }

  Uword getSize(Object o) {
	return o.vtable->functions.getSize(o);
  }

  void trace(Object o) {
	o.vtable->functions.trace(o);
  }

}

#endif
