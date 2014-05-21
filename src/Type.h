#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Primitives.h"

namespace octarine {

  struct Type;
  struct Context;

  Type* createType(Context* ctx, Uword size);
  Uword getSize(Context* ctx, Type* type);

}

#endif


