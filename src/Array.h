#ifndef OCT_ARRAY_H
#define OCT_ARRAY_H

#include "Primitives.h"

namespace octarine {

    static Array createArray(ThreadContext tc, Type elementType);
    static Array conj(Array a, Any val);

}

#endif
