#ifndef OCT_MEMORYMANAGER_H
#define OCT_MEMORYMANAGER_H

#include "Primitives.h"
#include "Typedefs.h"

namespace octarine {

    MemoryManager createMemoryManager();

    void destroyMemoryManager(MemoryManager mm);
    
    Object allocRaw(MemoryManager mm, Uword size);
    
    Object alloc(ThreadContext tc, MemoryManager mm, Type t);

}

#endif

