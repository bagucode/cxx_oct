#ifndef OCT_RUNTIME_H
#define OCT_RUNTIME_H

#include "Typedefs.h"

namespace octarine {

    Runtime createRuntime();

    void destroyRuntime(Runtime rt);

    void registerFunction(ThreadContext tc, Namespace ns, Function f);
    
    Object eval(ThreadContext tc, String source);
    
    Object intern(ThreadContext tc, Object o);

}

#endif
