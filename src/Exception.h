#ifndef OCT_EXCEPTION_H
#define OCT_EXCEPTION_H

#include "Typedefs.h"

namespace octarine {
    
    Exception createException(ThreadContext tc, String message);
    String getMessage(ThreadContext tc, Exception e);
    
}

#endif

