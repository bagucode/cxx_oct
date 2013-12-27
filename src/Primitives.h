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
    
    template <typename T>
    struct Array_t {
        Uword size;
        T data[];
    };
    
    template <typename T>
    using Array = Array_t<T>*;
    
    struct Any {
        Type type;
        void* object;
    };
    
    struct String_t;
    typedef String_t* String;
    
    struct FunctionSignature_t;
    typedef FunctionSignature_t* FunctionSignature;
    
    struct Namespace_t;
    typedef Namespace_t* Namespace;
    
    struct Symbol_t;
    typedef Symbol_t* Symbol;
    
    struct Runtime_t;
    typedef Runtime_t* Runtime;
    
    struct ThreadContext_t;
    typedef ThreadContext_t* ThreadContext;
    
    // Static type references for the primitive types
	Type U8Type;
	Type I8Type;
	Type U16Type;
	Type I16Type;
	Type U32Type;
	Type I32Type;
	Type U64Type;
	Type I64Type;
	Type F32Type;
	Type F64Type;
	Type BoolType;
	Type CharType;
	Type UwordType;
	Type WordType;
	Type AddressType;

}

#endif
