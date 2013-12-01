#include "Runtime.h"
#include "Primitives.h"
#include "MemoryManager.h"

#include <unordered_map>

namespace octarine {

	struct RuntimeT {
        MemoryManager sharedMemory;
	};

    Runtime createRuntime() {
        return nullptr;
    }

	void destroyRuntime(Runtime rt) {
        
    }

	void registerFunction(ThreadContext tc, Runtime rt, Namespace ns, Function f) {

	}
}
