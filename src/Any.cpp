#include "Any.h"
#include "ThreadContext.h"
#include "Function.h"
#include "Symbol.h"

namespace octarine {
    
    static Type typeOf(Any a) {
        return a.type;
    }
    
    static void init(Any a) {
        a.type = nullptr;
        a.object = nullptr;
    }
    
    static void destroy(Any a) {
        ThreadContext tc = currentContext();
        Namespace ns = currentNamespace(tc);
        Symbol destroyName = createSymbol(tc, createString(tc, "destroy"));
        
        Address fn = findFunction(ns, destroyName, <#FunctionSignature fs#>)
    }
    
    static Uword hash(Any a) {
    }
    
    static Bool equals(Any x, Any y) {
    }
    
    static void trace(Any a, MemoryManager mm) {
    }
    
}
