#ifndef OCT_FUNCTION_H
#define OCT_FUNCTION_H

#include "Primitives.h"

namespace octarine {

    static FunctionSignature createFunctionSignature(ThreadContext tc, Array parameterTypes, Array returnTypes);
    static Address findFunction(Namespace ns, Symbol name, FunctionSignature fs);
    
    // Finds a function in the current namespace
    static Address findFunction(const char* name, Type* paramTypes, Uword numParams, Type* retTypes, Uword numRets);

}

#endif
