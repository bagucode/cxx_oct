#include "String.h"
#include "Primitives.h"
#include "ThreadContext.h"

#include <cstring>
#include <memory>

namespace octarine {
    
    struct StringT {
        Uword length;
        U8* utf8data; // Just use platform strings?
    };

    String createString(ThreadContext tc, const char* cstr) {
        return nullptr;
    }
    
    Uword hash(ThreadContext tc, String s) {
        return 0;
    }
    
    Bool equals(ThreadContext tc, String s1, String s2) {
        return False;
    }

}
