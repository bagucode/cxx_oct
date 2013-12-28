#ifndef OCT_THREADCONTEXT_H
#define OCT_THREADCONTEXT_H

#include "Primitives.h"

namespace octarine {

    ThreadContext currentContext();
    Namespace currentNamespace(ThreadContext tc);
    Runtime runtime(ThreadContext tc);
    MemoryManager contextMemoryManager(ThreadContext tc);

}

#endif

