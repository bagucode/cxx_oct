#ifndef OCT_FUNCTION_H
#define OCT_FUNCTION_H

#include "Primitives.h"

namespace octarine {

    static FunctionSignature createFunctionSignature(ThreadContext tc, Array parameterTypes, Array returnTypes);
    static Address findFunction(Namespace ns, Symbol name, FunctionSignature fs);

}

#endif
