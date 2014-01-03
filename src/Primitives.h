#ifndef OCT_PRIMITIVES_H
#define OCT_PRIMITIVES_H

#ifdef _WIN32
#define WINDOWS
#include <Windows.h>

namespace octarine {

	typedef __int8           I8;
	typedef unsigned __int8  U8;
	typedef __int16          I16;
	typedef unsigned __int16 U16;
	typedef __int32          I32;
	typedef unsigned __int32 U32;
	typedef __int64          I64;
	typedef unsigned __int64 U64;
	typedef float            F32;
	typedef double           F64;

	typedef U8 Bool;
#define True 1
#define False 0

	typedef I32 Char;

#ifdef _WIN64
#define OCT64
	typedef I64 Word;
	typedef U64 Uword;
#else
#define OCT32
	typedef I32 Word;
	typedef U32 Uword;
#endif

	typedef void* Address;

#ifdef _DEBUG
#define DEBUG
#endif

}

#elif defined (__APPLE__)

#define MACOSX

#include <inttypes.h>

namespace octarine {

	typedef int8_t   I8;
	typedef uint8_t  U8;
	typedef int16_t  I16;
	typedef uint16_t U16;
	typedef int32_t  I32;
	typedef uint32_t U32;
	typedef int64_t  I64;
	typedef uint64_t U64;
	typedef float    F32;
	typedef double   F64;

	typedef U8 Bool;
#define True 1
#define False 0

	typedef I32 Char;

#ifdef __LP64__
#define OCT64
	typedef I64 Word;
	typedef U64 Uword;
#else
#define OCT32
	typedef I32 Word;
	typedef U32 Uword;
#endif

	typedef void* Address;

}

#else

#define LINUX

#include <inttypes.h>

namespace octarine {

	typedef int8_t   I8;
	typedef uint8_t  U8;
	typedef int16_t  I16;
	typedef uint16_t U16;
	typedef int32_t  I32;
	typedef uint32_t U32;
	typedef int64_t  I64;
	typedef uint64_t U64;
	typedef float    F32;
	typedef double   F64;

	typedef U8 Bool;
#define True 1
#define False 0

	typedef I32 Char;

#ifdef __LP64__
#define OCT64
	typedef I64 Word;
	typedef U64 Uword;
#else
#define OCT32
	typedef I32 Word;
	typedef U32 Uword;
#endif

	typedef void* Address;

#ifndef NDEBUG
#define DEBUG
#endif

}

#endif

namespace octarine {

    // Typedefs
    
    struct Type_t;
    typedef Type_t* Type;
    
    struct MemoryManager_t;
    typedef MemoryManager_t* MemoryManager;
    
    struct Field_t;
    typedef Field_t* Field;
    
    struct Array_t;
    typedef Array_t* Array;
    
    struct Any {
        Type type;
        void* object;
    };
    
    struct String_t;
    typedef String_t* String;
    
    struct FunctionSignature_t;
    typedef FunctionSignature_t* FunctionSignature;
    
    struct Function_t;
    typedef Function_t* Function;
    
    struct Namespace_t;
    typedef Namespace_t* Namespace;
    
    struct Symbol_t;
    typedef Symbol_t* Symbol;
    
    struct Runtime_t;
    typedef Runtime_t* Runtime;
    
    struct ThreadContext_t;
    typedef ThreadContext_t* ThreadContext;
    
    struct Variant_t;
    typedef Variant_t* Variant;
    
    struct Nothing {
        Uword unused; // Just to make it have a size. Never used.
    };
    static const Nothing nil = {0};
    
    struct CallstubTable_t;
    typedef CallstubTable_t* CallstubTable;

}

#endif
