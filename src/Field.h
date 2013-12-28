#ifndef OCT_FIELD_H
#define OCT_FIELD_H

#include "Primitives.h"

namespace octarine {
    
    static Type typeOfField(Field f);
    static Uword offset(Field f);
    static Field createField(ThreadContext tc, Type t, Uword offset);
    
    static Type _typeOfField(ThreadContext tc);

}

#endif
