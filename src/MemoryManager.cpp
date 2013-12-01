#include "MemoryManager.h"
#include "Primitives.h"

namespace octarine {

    struct MemoryManagerT {
        Uword hello;
    };
    
    MemoryManager createMemoryManager() {
        MemoryManager mm = new MemoryManagerT;
        return mm;
    }

    void destroyMemoryManager(MemoryManager mm) {
        delete mm;
    }
    
    Object allocRaw(MemoryManager mm, Uword size) {
        return nullptr;
    }
    
    Object alloc(ThreadContext tc, MemoryManager mm, Type t) {
        return nullptr;
    }

}
