#ifndef OCT_RUNTIME
#define OCT_RUNTIME

#include "Heap.h"

namespace octarine {

  struct Context;
  struct Runtime;
  struct Protocol;
  struct Symbol;
  struct Type;

  Runtime* createRuntime();

  Heap getHeap(Context* ctx, Runtime* rt);

  Protocol* getProtocol(Context* ctx, Runtime* rt, Symbol* protocolName);

  Type* getType(Context* ctx, Runtime* rt, Symbol* typeName);

}

#endif
