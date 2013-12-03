#include "MemoryManager.h"
#include "Primitives.h"

namespace octarine {

    struct MemoryManagerT {
        Uword hello;
    };
    
    MemoryManager createMemoryManager() {
    }

    void destroyMemoryManager(MemoryManager mm) {
        delete mm;
    }
    
    Object allocRaw(MemoryManager mm, Uword size) {
        return (Object)::operator new(size);
    }
    
    Object alloc(ThreadContext tc, MemoryManager mm, Type t) {
        return nullptr;
    }

}
