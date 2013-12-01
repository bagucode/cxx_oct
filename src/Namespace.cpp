#include "Namespace.h"
#include "ThreadContext.h"
#include "MemoryManager.h"
#include "String.h"
#include "Runtime.h"

namespace octarine {
    
    struct NamespaceT {
        String name;
    };

    Object bind(ThreadContext tc, Namespace ns, String name, Object obj) {
        return nullptr;
    }
    
}
