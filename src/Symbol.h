#ifndef OCT_SYMBOL_H
#define OCT_SYMBOL_H

#include "Primitives.h"

namespace octarine {
    
    static Symbol createSymbol(ThreadContext tc, String name);
    
    static Type _typeOfSymbol(ThreadContext tc);
    
}

#endif
