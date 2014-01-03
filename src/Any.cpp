#include "Any.h"
#include "Function.h"
#include "Type.h"
#include "Variant.h"
#include "Nothing.h"
#include "ThreadContext.h"
#include "CallstubTable.h"

#include "LLVMIncludes.h"

namespace octarine {

    static void init(Any a) {
        a.type = _typeOfNothing(currentContext());
        a.object = (void*)&nil;
    }
    
    static void destroy(Any a) {
        if(a.object == &nil) {
            return;
        }
        if(typeKind(a.type) == TypeKindVariant) {
            a = variantValue((Variant)a.object);
        }
        // TODO: find callstub!
        // First find function and then use the function to look up the stub?
        callstubSig dtor = nullptr; //(callstubSig)findFunction("destroy", (Type[]){a.type}, 1, {}, 0);
        dtor(a);
    }

    static Uword hash(Any a) {
        return 0;
    }
    
    static Bool equals(Any x, Any y) {
        return False;
    }
    
    static void trace(Any a, MemoryManager mm) {
    }
    
}
