
// Primitive values

#ifdef _WIN32
#define WINDOWS
#include <Windows.h>

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

#elif defined (__APPLE__)

#define MACOSX

#include <inttypes.h>

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
    
#else

#define LINUX

#include <inttypes.h>

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
    
#endif

// Includes

#include <stdio.h>

// Typedefs

struct StructField;

struct StructInfo_t;
typedef struct StructInfo_t* StructInfo;
    
struct Type_t;
typedef struct Type_t* Type;
    
struct RefType_t;
typedef struct RefType_t* RefType;

struct ValType_t;
typedef struct ValType_t* ValType;

struct VarType_t;
typedef struct VarType_t VarType;

struct Array_t;
typedef struct Array_t* Array;

// Structure definitions

struct StructField {
    Type type;
    Uword offset;
};

struct StructInfo_t {
    Array structFields;
    Uword alignment;
    Uword size;
};

#define TYPE_VARIANT_REF 0
#define TYPE_VARIANT_VAL 1
#define TYPE_VARIANT_VAR 2

struct Type_t {
    Uword variant;
    union {
        
    };
};

int main(int argc, char* argv[]) {
    
    printf("Hello World!\n");
    
    return 0;
    
}

