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

template <typename T, Uword size = 0>
struct Array {
	typedef typename T ElementType;

	T data[size];
};

template <typename T>
struct Array<T, 0> {
	typedef typename T ElementType;

	Uword size;
	T* data;
};

struct Type;

struct String {
	Uword size;
	const char* data; // TODO: Proper string. Probably use OS string types, they have unicode support.
};

struct Field {
	Type* type;
	Uword kind;
	Uword offset;
	String name;
};

struct Type {
	Uword size;
	Uword alignment;
	Array<Field> fields;
};

struct TypeProtocol {
	typedef Type SelfType;

	Uword(*getSize)(Type* self);
	Uword(*getAlignment)(Type* self);
	Array<Field>(*getFields)(Type* self);
};

template <typename PFns>
struct VTable {
	Type* type;
	PFns fns;
};

template <typename PFns>
struct ProtocolObject {
	typename PFns::SelfType* self;
	VTable<PFns>* vtable;
};


#define call(Function, ProtocolObject, ...) ProtocolObject.vtable->fns.Function(ProtocolObject.self, __VA_ARGS__)

#include <iostream>

static Uword testFn(Type* ti) {
	return ti->alignment;
}

int main() {
	ProtocolObject<TypeProtocol> aProtocolObject;
	Type t1;
	t1.alignment = sizeof(Uword);
	aProtocolObject.self = &t1;
	VTable<TypeProtocol> vt1;
	vt1.fns.getAlignment = testFn;
	aProtocolObject.vtable = &vt1;
	std::cout << "Alignment: " << call(getAlignment, aProtocolObject) << std::endl;
}


