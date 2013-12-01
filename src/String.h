#ifndef OCT_STRING_H
#define OCT_STRING_H

#include "Typedefs.h"
#include "Primitives.h"

namespace octarine {
    
    String createString(ThreadContext tc, const char* cstr);
    
    String intern(Runtime rt, String s);
    
    Uword hash(ThreadContext tc, String s);
    
    Bool equals(ThreadContext tc, String s);

}

#endif
