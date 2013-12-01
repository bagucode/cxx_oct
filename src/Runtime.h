#ifndef OCT_RUNTIME_H
#define OCT_RUNTIME_H

#include "Typedefs.h"

namespace octarine {

    Runtime createRuntime();
    void destroyRuntime(Runtime rt);

    void registerFunction(ThreadContext tc, Runtime rt, Namespace ns, Function f);

}

#endif
