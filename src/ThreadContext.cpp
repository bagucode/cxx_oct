#include "ThreadContext.h"
#include "ThreadLocalStorage.h"
#include "Exception.h"
#include "MemoryManager.h"

namespace octarine {
    
    static TLS tls;
    
    struct ThreadContextT {
        MemoryManager memoryManager;
    };
    
    ThreadContext createThreadContext(MemoryManager mm) {
        ThreadContext tc = (ThreadContext)allocRaw(mm, sizeof(ThreadContextT));
        tc->memoryManager = mm;
        tls.set(tc);
        return tc;
    }

    void destroyThreadContext(ThreadContext tc) {
        tls.set(nullptr);
        destroyMemoryManager(tc->memoryManager);
    }

    ThreadContext currentThreadContext() {
        return (ThreadContext)tls.get();
    }
    
}
