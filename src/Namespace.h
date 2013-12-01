#ifndef OCT_NAMESPACE_H
#define OCT_NAMESPACE_H

#include "Typedefs.h"

namespace octarine {

    Namespace createNamespace(ThreadContext tc, String name);
    
    Object bind(ThreadContext tc, Namespace ns, String name, Object obj);
    
}

#endif
