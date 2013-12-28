#include "Any.h"
#include "ThreadContext.h"
#include "Function.h"
#include "Symbol.h"
#include "String.h"
#include "MemoryManager.h"
#include "Array.h"
#include "Type.h"
#include "Variant.h"

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
        struct _r {
            String destroyStr;
            Symbol destroyName;
            Array params;
            Array rets;
            FunctionSignature sig;
        } r;
        _zeroRoots(&r);
        
        ThreadContext tc = currentContext();
        MemoryManager mm = contextMemoryManager(tc);
        _ScopedRoots<_r> _sr(mm, &r);
        
        Namespace ns = currentNamespace(tc);
        r.destroyStr = createString(tc, "destroy");
        r.destroyName = createSymbol(tc, r.destroyStr);
        r.rets = createArray(tc, _typeOfType(tc));
        r.params = conj(r.rets, {_typeOfType(tc), _typeOfString(tc)});
        r.sig = createFunctionSignature(tc, r.params, r.rets);
        dtorSig dtor = (dtorSig)findFunction(ns, r.destroyName, r.sig);
        dtor(a.object);
    }
    
    static Uword hash(Any a) {
    }
    
    static Bool equals(Any x, Any y) {
    }
    
    static void trace(Any a, MemoryManager mm) {
    }
    
}
