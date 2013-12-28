#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Primitives.h"

namespace octarine {
    
    // TODO: Change these to a variant type? Not very easy to encode in C...
    static const U8 TypeKindReference = 0;
    static const U8 TypeKindValue = 1;
    static const U8 TypeKindVariant = 2;
    
    // For a variant type we need a function that returns the possible values?
    // It should be possible to mix values like different integers with instances
    // of objects of a number of allowed types
    // Something like this:
    // For use as a C-style enum, symbols could be used
    // (variant 'Ref 'Val 'Var)
    // For use as a discriminated union, for example "Option", types could be specified
    // (variant Nothing String)
    // Or a mix, this variant could hold one of a few specified integers or symbols or an instance of Nothing or an instance of String
    // (variant 10 24 42 'Foo 'Bar Nothing String)
    
    static Uword size(Type t);
    static Uword alignment(Type t);
    static Array fields(Type t);
    static U8 typeKind(Type t);
    
    static Type _typeOfType(ThreadContext tc);

}

#endif
