#ifndef OCT_THREADCONTEXT_H
#define OCT_THREADCONTEXT_H

#include "Typedefs.h"

namespace octarine {

    ThreadContext createThreadContext(Runtime rt, MemoryManager mm, Namespace initialNs);
    
    void destroyThreadContext(ThreadContext tc);
    
    ThreadContext getThreadContext();
    
    Runtime getRuntime(ThreadContext tc);
    
    MemoryManager getMemoryManager(ThreadContext tc);
    
    Namespace getNamespace(ThreadContext tc);
    
    Namespace setNamespace(ThreadContext tc, Namespace ns);

}

#endif

