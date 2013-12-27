#ifndef OCT_ANY_H
#define OCT_ANY_H

#include "Primitives.h"

namespace octarine {

    static Type typeOf(Any a);
    static void init(Any a);
    static void destroy(Any a);
    static Uword hash(Any a);
    static Bool equals(Any x, Any y);
    static void trace(Any a, MemoryManager mm);

}

#endif
