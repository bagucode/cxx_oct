#ifndef OCT_SYMBOL_H
#define OCT_SYMBOL_H

namespace octarine {

  struct Symbol;
  struct String;

  Symbol* createSymbol(Context* ctx, String* name);

  Object asObject(Context* ctx, Symbol* sym);

  Symbol* asSymbol(Context* ctx, Object obj);

}

#endif

