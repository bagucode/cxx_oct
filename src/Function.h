#ifndef OCT_FUNCTION_H
#define OCT_FUNCTION_H

#include "Primitives.h"

namespace octarine {

    Address findFunction(Namespace ns, Symbol name, FunctionSignature fs);

}

#endif
