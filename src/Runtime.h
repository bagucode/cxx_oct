#ifndef OCT_RUNTIME_H
#define OCT_RUNTIME_H

#include "Primitives.h"

#include "LLVMIncludes.h"

namespace octarine {

    llvm::LLVMContext* llvmContext(Runtime rt);
    llvm::Module* llvmJitModule(Runtime rt);
    llvm::ExecutionEngine* llvmExecutionEngine(Runtime rt);
    
    CallstubTable callstubTable(Runtime rt);
    
}

#endif
