#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Primitives.h"

namespace octarine {
    
    static Uword size(Type t);
    static Uword alignment(Type t);
    static Array<Field> fields(Type t);

}

#endif
