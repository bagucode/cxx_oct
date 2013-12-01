#ifndef OCT_OBJECT_H
#define OCT_OBJECT_H

#include "Typedefs.h"

namespace octarine {

	Type getType(ThreadContext tc, Object obj);
    
    Uword hash(ThreadContext tc, Object o);
    
    Bool equals(ThreadContext tc, Object o1, Object o2);
    
    Object makeShared(ThreadContext tc, Object o);

}

#endif
