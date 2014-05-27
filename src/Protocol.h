#ifndef OCT_PROTOCOL
#define OCT_PROTOCOL

#include "Object.h"

namespace octarine {

  struct Protocol;
  struct Array;
  struct Symbol;

  Protocol* createProtocol(Context* ctx, Symbol* name, Array* signatures);

  Object asObject(Context* ctx, Protocol* protocol);

  Protocol* asProtocol(Context* ctx, Object obj);

}

#endif
