#ifndef OCT_THREADCONTEXT_H
#define OCT_THREADCONTEXT_H

#include "Typedefs.h"

namespace octarine {

    ThreadContext createThreadContext(MemoryManager mm);
    void destroyThreadContext(ThreadContext tc);
    
    ThreadContext getCurrentThreadContext();

}

#endif

