#include "Any.h"
#include "Function.h"
#include "Type.h"
#include "Variant.h"
#include "LLVMIncludes.h"

namespace octarine {

    static void init(Any a) {
        a.type = NothingType;
        a.object = (void*)&nil;
    }
    
    typedef void(*dtorSig)(void*);
    
    static void destroy(Any a) {
        if(a.object == &nil) {
            return;
        }
        if(typeKind(a.type) == TypeKindVariant) {
            a = variantValue((Variant)a.object);
        }
        dtorSig dtor = (dtorSig)findFunction("destroy", (Type[]){a.type}, 1, {}, 0);
        dtor(a.object);
    }
    
    static Uword hash(Any a) {
        
    }
    
    static Bool equals(Any x, Any y) {
    }
    
    static void trace(Any a, MemoryManager mm) {
    }
    
}
