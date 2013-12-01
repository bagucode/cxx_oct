#include "Runtime.h"
#include "Primitives.h"
#include "MemoryManager.h"
#include "ThreadContext.h"
#include "String.h"
#include "Namespace.h"
#include "Object.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <mutex>

namespace std {
    
    template <>
    struct hash<octarine::String> {
        std::size_t operator()(const octarine::String& s) const {
            return octarine::hash(octarine::getThreadContext(), s);
        }
    };
    
    template<>
    struct equal_to<octarine::String> {
        bool operator()(const octarine::String& s1, const octarine::String& s2) const {
            return octarine::equals(octarine::getThreadContext(), s1, s2);
        }
    };
    
    template<>
    struct hash<octarine::Object> {
        std::size_t operator()(const octarine::Object& o) const {
            return octarine::hash(octarine::getThreadContext(), o);
        }
    };
    
    template<>
    struct equal_to<octarine::Object> {
        bool operator()(const octarine::Object& o1, const octarine::Object& o2) {
            return octarine::equals(octarine::getThreadContext(), o1, o2);
        }
    };
    
}

namespace octarine {

    struct BuiltInTypes {
        Type namespaceType;
    };
    
	struct RuntimeT {
        std::mutex namespaceMutex;
        std::unordered_map<String, Namespace> namespaces;
        std::mutex threadContextMutex;
        std::vector<ThreadContext> threadContexts;
        std::mutex internedObjectsMutex;
        std::unordered_set<Object> internedObjects;
        BuiltInTypes builtInTypes;
	};

    Runtime createRuntime() {
        Runtime rt = new RuntimeT;
        // Create main thread context, with its own memory manager
        ThreadContext mainTc = createThreadContext(rt, createMemoryManager(), nullptr);
        rt->threadContexts.push_back(mainTc);
        // Create main namespace
        setNamespace(mainTc, createNamespace(rt, createString(mainTc, "octarine")));
        return rt;
    }

	void destroyRuntime(Runtime rt) {
        std::for_each(rt->threadContexts.begin(), rt->threadContexts.end(), destroyThreadContext);
        delete rt;
    }

    void registerFunction(ThreadContext tc, Namespace ns, Function f) {
        
    }
    
    Object eval(ThreadContext tc, String source) {
        return nullptr;
    }
    
    Namespace createNamespace(Runtime rt, String name) {
        MemoryManager mm = getMemoryManager(tc);
        Runtime rt = getRuntime(tc);
        Namespace ns = (Namespace)alloc(tc, mm, getNamespaceType(rt));
        ns->name = name;
        rt->namespaces[octarineStr] = octarineNs;
        return (Namespace)intern(tc, (Object)ns);
    }
    
    Type getNamespaceType(Runtime rt) {
        return rt->builtInTypes.namespaceType;
    }
    
    Object intern(ThreadContext tc, Object o) {
        Runtime rt = getRuntime(tc);
        std::lock_guard<std::mutex> lock(rt->internedObjectsMutex);
        std::unordered_set<Object>::const_iterator i = rt->internedObjects.find(o);
        if(i == rt->internedObjects.end()) {
            // Not found, copy to global memory and insert.
            return nullptr;
        }
        else {
            return *i;
        }
    }

}

