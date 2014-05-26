#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "ObjectFunctions.h"

namespace octarine {

  struct Type;
  struct Context;
  struct Array;

  Type* createType(Context* ctx, Uword size, Array* fields, ObjectFunctions ofns);
  Uword getSize(Context* ctx, Type* type);

}

#endif


