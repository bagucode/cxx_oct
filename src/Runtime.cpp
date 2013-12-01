#include "Runtime.h"
#include "Primitives.h"
#include "MemoryManager.h"
#include "ThreadContext.h"

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace octarine {

	struct RuntimeT {
        MemoryManager sharedMemory;
        std::vector<ThreadContext> threadContexts;
	};

    Runtime createRuntime() {
        Runtime rt = new RuntimeT;
        rt->sharedMemory = createMemoryManager();
        // Create main thread context, with it's own memory manager
        rt->threadContexts.push_back(createThreadContext(rt, createMemoryManager()));
        return rt;
    }

	void destroyRuntime(Runtime rt) {
        destroyMemoryManager(rt->sharedMemory);
        std::for_each(rt->threadContexts.begin(), rt->threadContexts.end(), destroyThreadContext);
        delete rt;
    }

    void registerFunction(ThreadContext tc, Namespace ns, Function f) {
        
    }
    
    Object eval(ThreadContext tc, String source) {
        return nullptr;
    }
}
