#include "Type.h"
#include "Field.h"
#include "Array.h"
#include "Context.h"
#include "Runtime.h"
#include "HeapFunctions.h"

namespace octarine {

  struct Type {
	Uword size;
	Array* fields;
  };

  Type typeOfType = { // TODO: how do we do with these?
	sizeof(Type),
	0
  };

  Type* createType(Context* ctx, Uword size, Array* fields, ObjectFunctions ofns) {
	Runtime* rt = getRuntime(ctx);
	Heap heap = getHeap(ctx, rt); // Types are always global
	Object typeObj = alloc(ctx, heap, &typeOfType);
	Type* t = (Type*)typeObj.object;
	t->size = size;
	t->fields = fields;
	return t;
  }

  Uword getSize(Context* ctx, Type* type) {
	return type->size;
  }

}
