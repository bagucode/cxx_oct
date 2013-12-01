#include "ThreadContext.h"
#include "ThreadLocalStorage.h"
#include "Exception.h"
#include "MemoryManager.h"

namespace octarine {
    
    static TLS tls;
    
    struct ThreadContextT {
        Runtime runtime;
        MemoryManager memoryManager;
    };
    
    ThreadContext createThreadContext(Runtime rt, MemoryManager mm) {
        ThreadContext tc = (ThreadContext)allocRaw(mm, sizeof(ThreadContextT));
        tc->runtime = rt;
        tc->memoryManager = mm;
        tls.set(tc);
        return tc;
    }

    void destroyThreadContext(ThreadContext tc) {
        tls.set(nullptr);
        destroyMemoryManager(tc->memoryManager);
    }

    ThreadContext getThreadContext() {
        return (ThreadContext)tls.get();
    }
    
    Runtime getRuntime(ThreadContext tc) {
        return tc->runtime;
    }

}
