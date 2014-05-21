#include "Type.h"
#include "Field.h"
#include "Array.h"
#include "Context.h"
#include "Runtime.h"
#include "HeapFunctions.h"
#include "ObjectFunctions.h"

namespace octarine {

  struct Type {
	Uword size;
	Array* fields;
	ObjectFunctions objectFns;
  };

  Type typeOfType = { // TODO: how do we do with these?
	sizeof(Type),
	0,
	{
	  0,
	  0,
	  0,
	  0
	}
  };

  Type* createType(Context* ctx, Uword size) {
	Runtime* rt = getRuntime(ctx);
	Heap heap = getHeap(ctx, rt); // Types are always global
	Object typeObj = alloc(ctx, heap, &typeOfType);
	return (Type*)typeObj.object;
  }

  Uword getSize(Context* ctx, Type* type) {
	return type->size;
  }

}
