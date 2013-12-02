#ifndef OCTARINE_LANG
#define OCTARINE_LANG

// Table of contents
// ## 00 Macro Definitions
// ## 10 Includes
// ## 20 Type declarations and typedefs
// ## 30 Function declarations
// ## 40 Type definitions
// ## 50 Function definitions
// ## 60 C API

// ## 00 Macro Definitions

#ifdef _WIN32
#define WINDOWS

#ifdef _DEBUG
#define DEBUG
#endif

#ifdef _WIN64
#define OCT64
#else
#define OCT32
#endif

#elif defined (__APPLE__)
#define MACOSX

#ifdef __LP64__
#define OCT64
#else
#define OCT32
#endif

#else
#define LINUX

#ifndef NDEBUG
#define DEBUG
#endif

#ifdef __LP64__
#define OCT64
#else
#define OCT32
#endif

#endif

#define True 1
#define False 0

// ## 10 Includes

#ifdef WINDOWS
#include <Windows.h>
#else
#include <inttypes.h>
#endif

namespace octarine {
    
    // ## 20 Type declarations and typedefs
    
#ifdef WINDOWS
	typedef __int8           I8;
	typedef unsigned __int8  U8;
	typedef __int16          I16;
	typedef unsigned __int16 U16;
	typedef __int32          I32;
	typedef unsigned __int32 U32;
	typedef __int64          I64;
	typedef unsigned __int64 U64;
#else
    typedef int8_t   I8;
	typedef uint8_t  U8;
	typedef int16_t  I16;
	typedef uint16_t U16;
	typedef int32_t  I32;
	typedef uint32_t U32;
	typedef int64_t  I64;
	typedef uint64_t U64;
#endif
	typedef float    F32;
	typedef double   F64;
	typedef U8 Bool;
	typedef I32 Char;
#ifdef OCT64
	typedef I64 Word;
	typedef U64 Uword;
#else
	typedef I32 Word;
	typedef U32 Uword;
#endif
	typedef void* Address;

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

    // ## 30 Function declarations
    
    MemoryManager createMemoryManager();
    
    void destroyMemoryManager(MemoryManager mm);
    
    Object allocRaw(MemoryManager mm, Uword size);
    
    Object alloc(ThreadContext tc, MemoryManager mm, Type t);
    
    Namespace createNamespace(Runtime rt, String name);
    
    Object bind(ThreadContext tc, Namespace ns, String name, Object obj);
    
    Type getNamespaceType(Runtime rt);
    
    Bool equals(ThreadContext tc, Namespace ns, Object obj);

	Type getType(ThreadContext tc, Object obj);
    
    Uword hash(ThreadContext tc, Object o);
    
    Bool equals(ThreadContext tc, Object o1, Object o2);
    
    Object makeShared(ThreadContext tc, Object o);

    Runtime createRuntime();
    
    void destroyRuntime(Runtime rt);
    
    void registerFunction(ThreadContext tc, Namespace ns, Function f);
    
    Object eval(ThreadContext tc, String source);
    
    Object intern(ThreadContext tc, Object o);

    String createString(ThreadContext tc, const char* cstr);
    
    Uword hash(ThreadContext tc, String s);
    
    Bool equals(ThreadContext tc, String s1, String s2);

    ThreadContext createThreadContext(Runtime rt, MemoryManager mm, Namespace initialNs);
    
    void destroyThreadContext(ThreadContext tc);
    
    ThreadContext getThreadContext();
    
    Runtime getRuntime(ThreadContext tc);
    
    MemoryManager getMemoryManager(ThreadContext tc);
    
    Namespace getNamespace(ThreadContext tc);
    
    Namespace setNamespace(ThreadContext tc, Namespace ns);

    Exception createException(ThreadContext tc, String message);
    
    String getMessage(ThreadContext tc, Exception e);

    // ## 40 Type definitions

    // ## 50 Function definitions
    
    
}

// ## 60 C API

#endif
