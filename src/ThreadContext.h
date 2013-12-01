#ifndef OCT_THREADCONTEXT_H
#define OCT_THREADCONTEXT_H

#include "Typedefs.h"

namespace octarine {

    ThreadContext createThreadContext(Runtime rt, MemoryManager mm);
    
    void destroyThreadContext(ThreadContext tc);
    
    ThreadContext getThreadContext();
    
    Runtime getRuntime(ThreadContext tc);

}

#endif

