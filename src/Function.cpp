#include "Function.h"
#include "MemoryManager.h"
#include "ThreadContext.h"
#include "Symbol.h"
#include "String.h"
#include "Array.h"
#include "Type.h"

namespace octarine {
    
    static FunctionSignature createFunctionSignature(ThreadContext tc, Array parameterTypes, Array returnTypes) {
        return nullptr;
    }
    
    static Address findFunction(Namespace ns, Symbol name, FunctionSignature fs) {
        return nullptr;
    }
    
    // Finds a function in the current namespace
    static Address findFunction(const char* name, Type* paramTypes, Uword numParams, Type* retTypes, Uword numRets) {
        struct _r {
            String nameStr;
            Symbol nameSym;
            Array params;
            Array rets;
            FunctionSignature sig;
        } r;
        _zeroRoots(&r);
        
        ThreadContext tc = currentContext();
        _ScopedRoots<_r> _sr(contextMemoryManager(tc), &r);
        
        r.nameStr = createString(tc, name);
        r.nameSym = createSymbol(tc, r.nameStr);
        
        Type typeType = _typeOfType(tc);
        
        r.params = createArray(tc, typeType);
        r.rets = r.params;
        
        for (Uword i = 0; i < numParams; ++i) {
            r.params = conj(r.params, {typeType, paramTypes[i]});
        }
        
        for (Uword i = 0; i < numRets; ++i) {
            r.rets = conj(r.rets, {typeType, retTypes[i]});
        }

        r.sig = createFunctionSignature(tc, r.params, r.rets);

        Namespace ns = currentNamespace(tc);
        return findFunction(ns, r.nameSym, r.sig);
    }
    
}
