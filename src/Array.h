#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Object.h"

namespace octarine {

  struct Type;
  struct Context;
  struct Array;

  Array* createArray(Context* ctx, Type* elementType, Uword size);

  Object asObject(Context* ctx, Array* arr);

  Array* asArray(Context* ctx, Object obj);

  Object getValue(Context* ctx, Array* arr, Uword index);

  void setValue(Context* ctx, Array* arr, Uword index, Object value);

  Uword getSize(Context* ctx, Array* arr);

  Type* getElementType(Context* ctx, Array* arr);

}

#endif
