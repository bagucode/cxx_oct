#include "MemoryManager.h"
#include "Primitives.h"

namespace octarine {

    struct MemoryManagerT {
        Uword hello;
    };
    
    MemoryManager createMemoryManager() {
        return nullptr;
    }

    void destroyMemoryManager(MemoryManager mm) {
        
    }
    
    Object allocRaw(MemoryManager mm, Uword size) {
        return nullptr;
    }
    
    Object alloc(ThreadContext tc, MemoryManager mm, Type t) {
        return nullptr;
    }

}
