#include "ThreadContext.h"
#include "ThreadLocalStorage.h"
#include "Exception.h"
#include "MemoryManager.h"

namespace octarine {
    
    static TLS tls;
    
    struct ThreadContextT {
        Runtime runtime;
        MemoryManager memoryManager;
        Namespace currentNs;
    };
    
    ThreadContext createThreadContext(Runtime rt, MemoryManager mm, Namespace initialNs) {
        ThreadContext tc = (ThreadContext)allocRaw(mm, sizeof(ThreadContextT));
        tc->runtime = rt;
        tc->memoryManager = mm;
        tc->currentNs = initialNs;
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
    
    MemoryManager getMemoryManager(ThreadContext tc) {
        return tc->memoryManager;
    }

    Namespace getNamespace(ThreadContext tc) {
        return tc->currentNs;
    }
    
    Namespace setNamespace(ThreadContext tc, Namespace ns) {
        Namespace old = tc->currentNs;
        tc->currentNs = ns;
        return old;
    }
}
