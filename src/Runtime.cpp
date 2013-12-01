#include "Runtime.h"
#include "Primitives.h"
#include "MemoryManager.h"
#include "ThreadContext.h"
#include "String.h"
#include "Namespace.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

namespace std {
    
    template <>
    struct hash<octarine::String> {
        std::size_t operator()(octarine::String s) const {
            return octarine::hash(octarine::getThreadContext(), s);
        }
    };
    
    template<>
    struct equal_to<octarine::String> {
        bool operator()(octarine::String s) const {
            return octarine::equals(octarine::getThreadContext(), s);
        }
    };
    
}

namespace octarine {

	struct RuntimeT {
        std::unordered_map<String, Namespace> namespaces;
        std::vector<ThreadContext> threadContexts;
        std::unordered_set<String> internedStrings;
	};

    Runtime createRuntime() {
        Runtime rt = new RuntimeT;
        // Create main thread context, with its own memory manager
        ThreadContext mainTc = createThreadContext(rt, createMemoryManager());
        rt->threadContexts.push_back(mainTc);
        // Create main namespace
        String octarineStr = createString(mainTc, "octarine");
        Namespace octarineNs = createNamespace(mainTc, octarineStr);
        rt->namespaces[octarineStr] = octarineNs;
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
}
