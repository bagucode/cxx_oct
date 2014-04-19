#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Protocol.h"

namespace octarine {

  struct ObjectFunctions;

  typedef Protocol<ObjectFunctions> Object;

  typedef Object (*ObjectCopyFn)();
  typedef Object (*ObjectCopyToHeapFn)();
  typedef Uword (*ObjectGetSizeFn)();
  typedef void (*ObjectTraceFn)();

  struct ObjectFunctions {
  private:
	ObjectCopyFn _copy;
	ObjectCopyToHeapFn _copyToHeap;
	ObjectGetSizeFn _getSize;
	ObjectTraceFn _trace;
  public:
	ObjectFunctions(ObjectCopyFn copy,
					ObjectCopyToHeapFn copyToHeap,
					ObjectGetSizeFn getSize,
					ObjectTraceFn trace):
	  _copy(copy),
	  _copyToHeap(copyToHeap),
	  _getSize(getSize),
	  _trace(trace) {
	}

	Object copy() {
	  return _copy();
	}

	Object copyToHeap() {
	  return _copyToHeap();
	}

	Uword getSize() {
	  return _getSize();
	}

	void trace() {
	  _trace();
	}
  };

}

#endif
