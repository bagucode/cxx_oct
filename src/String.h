#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Primitives.h"

namespace octarine {

  struct String;

  String* createString(Context* ctx, U8* utf8Data);

  Object asObject(Context* ctx, String* str);

  String* asString(Context* ctx, Object obj);

}

#endif
