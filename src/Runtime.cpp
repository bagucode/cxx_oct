#include "Runtime.h"
#include "Primitives.h"
#include "MemoryManager.h"

#include <unordered_map>

namespace octarine {

	struct RuntimeT {
        MemoryManager sharedMemory;
        
	};

    Runtime createRuntime() {
        Runtime rt = new RuntimeT;
        rt->sharedMemory = createMemoryManager();
        return rt;
    }

	void destroyRuntime(Runtime rt) {
        destroyMemoryManager(rt->sharedMemory);
        delete rt;
    }

    void registerFunction(ThreadContext tc, Namespace ns, Function f) {
        
    }
    
    Object eval(ThreadContext tc, String source) {
        return nullptr;
    }
}
