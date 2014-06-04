#include "LeakHeap.h"
#include "Type.h"
#include "ObjectFunctions.h"
#include "String.h"
#include "Symbol.h"
#include "Protocol.h"
#include "Context.h"
#include "Runtime.h"
#include <stdlib.h>

namespace octarine {

  struct LeakHeap {
	Uword size; // or something...
  };

  LeakHeap* createLeakHeap(Context* ctx) {
	LeakHeap* lh = (LeakHeap*)malloc(sizeof(LeakHeap));
	lh->size = 0;
	return lh;
  }

  void destroy(Context* ctx, LeakHeap* heap) {
	free(heap);
  }

  Object alloc(Context* ctx, LeakHeap* heap, Type* type, Object constructArg) {
	Uword size = getSize(ctx, type);
	Object o;
	o.object = malloc(size);
	Runtime* rt = getRuntime(ctx);
	String* objStr = createString(ctx, (U8*)"Object");
	Symbol* objSym = createSymbol(ctx, objStr);
	Protocol* objProtocol = getProtocol(ctx, rt, objSym);
	o.vtable = getVTable(ctx, objProtocol, type);
	construct(ctx, o, constructArg);
	heap->size += size;
	return o;
  }

  Address rawAlloc(Context* ctx, LeakHeap* heap, Uword size) {
  }

  void setType(Context* ctx, LeakHeap* heap, Address location, Type* type) {
  }

}
