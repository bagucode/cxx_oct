#ifndef OCT_FUNCTION_H
#define OCT_FUNCTION_H

#include "Primitives.h"

namespace octarine {

    static FunctionSignature createFunctionSignature(ThreadContext tc, Array parameterTypes, Array returnTypes);
    static Function findFunction(Namespace ns, Symbol name, FunctionSignature fs);
    
    // Finds a function in the current namespace
    static Function findFunction(const char* name, Type* paramTypes, Uword numParams, Type* retTypes, Uword numRets);

}

#endif
