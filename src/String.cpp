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

}
