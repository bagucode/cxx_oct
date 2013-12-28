#ifndef OCT_MEMORYMANAGER_H
#define OCT_MEMORYMANAGER_H

#include "Primitives.h"

#include <memory>

namespace octarine {

    static void pushRoots(MemoryManager mm, Address rootsBase, Uword nRoots);
    static void popRoots(MemoryManager mm);
    
    template <typename T>
    static void _zeroRoots(T* roots) {
        std::memset(roots, 0, sizeof(T) / sizeof(Address));
    }
    
    template <typename T>
    struct _ScopedRoots {
        _ScopedRoots(MemoryManager mm, T* roots) {
            this->mm = mm;
            pushRoots(mm, roots, sizeof(T) / sizeof(Address));
        }
        ~_ScopedRoots() {
            popRoots(mm);
        }

        MemoryManager mm;
    };

}

#endif
