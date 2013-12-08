#ifndef OCT_NAMESPACE_H
#define OCT_NAMESPACE_H

#include "Primitives.h"

namespace octarine {

    Namespace createNamespace(Runtime rt, String name);
    
    Object bind(ThreadContext tc, Namespace ns, String name, Object obj);
    
    Type getNamespaceType(Runtime rt);
    
    Bool equals(ThreadContext tc, Namespace ns, Object obj);
    
}

#endif
