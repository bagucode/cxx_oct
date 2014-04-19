#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Protocol.h"

namespace octarine {

  struct ObjectFunctions;
  //typedef Protocol<ObjectFunctions> Object;
  struct Object: public Protocol<ObjectFunctions> {
	public:
	Object(Address object, VTable<ObjectFunctions>* vtable):
	  Protocol(object, vtable) {
	}
  };

  struct ObjectFunctions {
	public:
	Object (*copy)();
	Object (*copyToHeap)();
	Uword (*getSize)();
	void (*trace)();
  };

}

#endif
