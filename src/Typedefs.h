#ifndef OCT_TYPEDEFS_H
#define OCT_TYPEDEFS_H

namespace octarine {
    
    struct FunctionT;
	typedef FunctionT* Function;

    struct MemoryManagerT;
    typedef MemoryManagerT* MemoryManager;

	struct NamespaceT;
	typedef NamespaceT* Namespace;

    struct ObjectT;
	typedef ObjectT* Object;

    struct ReaderT;
    typedef ReaderT* Reader;

	struct RuntimeT;
    typedef RuntimeT* Runtime;

	struct StringT;
    typedef StringT* String;

	struct ThreadContextT;
    typedef ThreadContextT* ThreadContext;

	struct TypeT;
	typedef TypeT* Type;

    struct ExceptionT;
    typedef ExceptionT* Exception;

}

#endif

