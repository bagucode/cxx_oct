#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Object.h"

namespace octarine {

  struct Type;
  struct Context;
  struct Array;

  void _initArrayType(Context* ctx);

  Array* createArray(Context* ctx, Type* elementType, Uword size);

  Object asObject(Context* ctx, Array* arr);

  Array* asArray(Context* ctx, Object obj);

}

#endif