
// Defines

#ifdef _WIN32
#define WINDOWS

#ifdef _WIN64
#define OCT64
#else
#define OCT32
#endif

#else // _WIN32

#ifdef __APPLE__
#define MACOSX
#else
#define LINUX
#endif

#ifdef __LP64__
#define OCT64
#else
#define OCT32
#endif

#endif // _WIN32

#ifdef _DEBUG
#define DEBUG
#endif

#ifndef NDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

// Includes

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

#ifdef WINDOWS
#include <Windows.h>
#endif

#ifndef WINDOWS
#include <pthread.h>
#include <inttypes.h>
#endif

// Primitive values

#ifdef WINDOWS

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

#else

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

#endif

typedef U8 Bool;
#define True 1
#define False 0

typedef I32 Char;

#ifdef OCT64
typedef I64 Word;
typedef U64 Uword;
#else
typedef I32 Word;
typedef U32 Uword;
#endif

typedef void* Address;

// Thread local storage

struct TLS_t {
#ifdef WINDOWS
    DWORD key;
#else
    pthread_key_t key;
#endif
};
typedef struct TLS_t TLS_t;
typedef TLS_t* TLS;

static void TLSCreate(TLS tls) {
#ifdef WINDOWS
    tls->key = TlsAlloc();
#else
    pthread_key_create(&tls->key, NULL);
#endif
}

static void TLSDestroy(TLS tls) {
#ifdef WINDOWS
    TlsFree(tls->key);
#else
    pthread_key_delete(tls->key);
#endif
}

static void TLSSet(TLS tls, void* value) {
#ifdef WINDOWS
    TlsSetValue(tls->key, value);
#else
    pthread_setspecific(tls->key, value);
#endif
}

static void* TLSGet(TLS tls) {
#ifdef WINDOWS
    return TlsGetValue(tls->key);
#else
    return pthread_getspecific(tls->key);
#endif
}

// Typedefs

struct StructField;
typedef struct StructField StructField;

struct StructInfo_t;
typedef struct StructInfo_t* StructInfo;

struct Type_t;
typedef struct Type_t Type;

struct RefType_t;
typedef struct RefType_t* RefType;

struct ValType_t;
typedef struct ValType_t* ValType;

struct VarType_t;
typedef struct VarType_t* VarType;

struct Array_t;
typedef struct Array_t* Array;

struct String_t;
typedef struct String_t* String;

struct Symbol_t;
typedef struct Symbol_t* Symbol;

struct Vector_t;
typedef struct Vector_t* Vector;

struct Object_t;
typedef struct Object_t* Object;

struct Value_t;
typedef struct Value_t Value;

struct NameValuePair;
typedef struct NameValuePair NameValuePair;

struct Namespace_t;
typedef struct Namespace_t* Namespace;

struct Context_t;
typedef struct Context_t* Context;

struct Runtime_t;
typedef struct Runtime_t* Runtime;

struct HeapBlock_t;
typedef struct HeapBlock_t* HeapBlock;

struct Heap_t;
typedef struct Heap_t* Heap;

struct BuiltinTypes_t;
typedef struct BuiltinTypes_t BuiltinTypes;

struct BuiltinProtocols_t;
typedef struct BuiltinProtocols_t BuiltinProtocols;

struct Nothing_t;
typedef struct Nothing_t* Nothing;

struct OpStackSlot_t;
typedef struct OpStackSlot_t OpStackSlot;

struct OpStack_t;
typedef struct OpStack_t* OpStack;

struct FunctionParameter_t;
typedef struct FunctionParameter_t* FunctionParameter;

struct FunctionSignature_t;
typedef struct FunctionSignature_t* FunctionSignature;

struct FunctionImplementation_t;
typedef struct FunctionImplementation_t* FunctionImplementation;

struct ProtocolFunction_t;
typedef struct ProtocolFunction_t* ProtocolFunction;

struct ProtocolSignature_t;
typedef struct ProtocolSignature_t* ProtocolSignature;

struct ProtocolImplementation_t;
typedef struct ProtocolImplementation_t* ProtocolImplementation;

struct Protocol_t;
typedef struct Protocol_t* Protocol;

struct Self_t;
typedef struct Self_t* Self;

// Protocols

struct Equals_t;
typedef struct Equals_t Equals;
typedef Bool(*EqualsFn)(Context, Address, Address);

// This is the C-signature of all interpreted ovm functions

typedef void(*octFn)(Context ctx);

// Structure definitions

#define TYPE_VARIANT_REF 0
#define TYPE_VARIANT_VAL 1
#define TYPE_VARIANT_VAR 2

struct Type_t {
    Uword variant;
    union {
        RefType ref;
        ValType val;
        VarType var;
    };
};

struct StructField {
    Type type;
    String name;
    Uword offset;
};

struct StructInfo_t {
    Array structFields;
    Uword alignment;
    Uword size;
};

struct RefType_t {
    StructInfo structInfo;
};

struct ValType_t {
    StructInfo structInfo;
};

struct VarType_t {
    Array variants; // Values or Types or a mix of both
    Uword alignment;
    Uword size;
};

struct Array_t {
    Type elementType;
    Uword size;
    Address data;
};

struct String_t {
    Uword size;
    Array utf8Data;
};

struct Symbol_t {
    String name;
};

struct Vector_t {
    Uword size;
    Array data; // size of data is total capacity
};

struct Object_t {
    Type type;
};

struct Value_t {
    Address data;
};

struct NameValuePair {
    String name;
    Value value;
};

struct Namespace_t {
    String name;
    Vector entries;
};

struct Nothing_t {
    Uword nothing; // Struct must have a size, but this member is never used.
};

// This is used to avoid doing type lookups in the built in functions,
// it is not strictly needed.
struct BuiltinTypes_t {
    struct BuiltinPrimitiveTypes_t {
        Type i8;
        Type u8;
        Type i16;
        Type u16;
        Type i32;
        Type u32;
        Type i64;
        Type u64;
        Type f32;
        Type f64;
        Type boolean;
        Type character;
        Type address;
        Type nothing;
        Type word;
        Type uword;
    } primitiveTypes;
    struct BuiltinValueTypes_t {
        Type value;
        Type structField;
        Type nameValuePair;
        Type opStackSlot;
        Type protocolFunction;
    } valueTypes;
    struct BuiltinVariadicTypes_t {
        Type type;
    } variadicTypes;
    struct BuiltinReferenceTypes_t {
        Type structInfo;
        Type refType;
        Type valType;
        Type varType;
        Type array;
        Type string;
        Type symbol;
        Type vector;
        Type object;
        Type namespace;
        Type runtime;
        Type context;
        Type opStack;
        Type functionSignature;
        Type protocol;
    } referenceTypes;
};

struct BuiltinProtocols_t {
    Protocol equals;
};

struct Runtime_t {
    Heap heap;
    BuiltinTypes builtinTypes;
    BuiltinProtocols builtinProtocols;
	Vector namespaces;
};

struct HeapBlock_t {
    Address start;
    Address end;
    Address pos;
    HeapBlock prev;
};

struct Heap_t {
    HeapBlock tail;
    Uword totalSize;
};

struct OpStackSlot_t {
    Type type;
    Address value;
};

struct OpStack_t {
    Uword slotTop;
    Array slots;
    Address dataTop;
    Array data;
};

struct Context_t {
    Runtime runtime;
    Heap heap;
    OpStack operandStack;
	Namespace currentNs;
};

struct FunctionParameter_t {
  String name;
  Type type;
  // TODO: store in metadata?
  Bool byRef;
  Bool isRead;
  Bool isWritten;
  Bool escapes;
};

struct FunctionSignature_t {
  String name;
  Vector arguments; // Vector<FunctionParameter>
  Vector returns;   // Vector<FunctionParameter>
  // TODO: store in metadata?
  Bool isPure;
  Bool allocatesMemory;
  Bool triggersGc; // redundant? Maybe allocatesMemory is enough?
};

struct FunctionImplementation_t {
  FunctionSignature signature;
  Address function; // octFn or a C function
  Bool isNative; // cdecl?
};

struct Function_t {
  String name;
  Vector implementations; // Vector<FunctionImplementation>
};

struct ProtocolSignature_t {
  String name;
  // The signatures below must have the Self placeholder as the
  // type of the first parameter and they must have at least one
  // parameter.
  Vector functions; // Vector<FunctionSignature>
};

struct ProtocolImplementation_t {
  ProtocolSignature signature;
  Type selfType;
  Vector functions; // Vector<FunctionImplementation>
};

struct Protocol_t {
  String name;
  Vector implementations; // Vector<ProtocolImplementation>
};

struct Equals_t {
    Address self;
    EqualsFn eq;
};

// Global variables

// If an ovm function is called as a callback from C code we may not be able to
// get the current context or runtime as a parameter so there has to be a way
// to globally get access to the current context. This thread local variable is
// used for that purpose. Since it is only thread local and not local to a
// specific Runtime instance this mechanism introduces a restriction that only
// one ovm Runtime may be attached to any given thread.
static TLS_t currentContext;

// Functions

// Forward declarations (just for the functions that need it)
static Uword TypeGetAllocationSize(Type type);
static Uword TypeGetFieldSize(Type type);
static Uword TypeGetAllocationAlignment(Type type);
static Uword TypeGetFieldAlignment(Type type);
static Bool TypeIsRefType(Type type);
static Bool TypeIsValType(Type type);
static Bool TypeIsVarType(Type type);
static Uword UwordAlignOn(Uword x, Uword alignOn);
static Type ArrayGetType(Context ctx);
static Uword ArrayGetSize(Array arr);
static Address ArrayGetFirstElement(Array arr);
static Type ObjectGetType(Address obj);
static Bool TypeEquals(Type x, Type y);
static Uword ObjectGetTotalSize(Type type, Bool asField, Address storeLocation, Uword alignmentAfterAllocation);
static void ObjectStorageSetup(Type objType, Address storeLocation, Bool asField, Uword alignmentAfterAllocation, Address* objectLocation, Address* nextStoreLocation);
static OpStack OpStackCreate(Context ctx, Heap heap);
static Runtime ContextGetRuntime(Context ctx);
static Heap RuntimeGetHeap(Runtime rt);
static Bool StringEquals(Context ctx, String s1, String s2);
static FunctionSignature FunctionSignatureCreate(Context ctx, String name, Vector argTypes, Vector retTypes);
static Uword VectorGetSize(Vector v);
static Array VectorGetBackingArray(Vector v);
static void OpStackPush(Context ctx, Heap heap, OpStack os, Type valueType, Address src);
static Address OpStackPeek(Context ctx, OpStack os, Type expectedType, Uword index);
static Bool FunctionImplementationIsNative(FunctionImplementation fi);
static Address FunctionImplementationGetFunctionPointer(FunctionImplementation fi);

// Heap

static Heap OvmHeapCreate(Uword initialSize) {
    const Uword extra = sizeof(struct Heap_t) + sizeof(struct HeapBlock_t);
    Address block = malloc(initialSize + extra);
	Heap heap;

    if(!block) {
        return NULL;
    }
    heap = (Heap)block;
    heap->tail = (Address)((U8*)block + sizeof(struct Heap_t));
    heap->totalSize = initialSize;

    heap->tail->start = (U8*)heap->tail + sizeof(struct HeapBlock_t);
    heap->tail->end = (U8*)heap->tail->start + initialSize;
    heap->tail->pos = heap->tail->start;
    heap->tail->prev = NULL;

    return heap;
}

static void OvmHeapDestroy(Heap heap) {
    HeapBlock block = heap->tail;
    while(block->prev) {
        HeapBlock prev = block->prev;
        free(block);
        block = prev;
    }
    free(heap);
}

static Address OvmHeapAllocRaw(Heap heap, Uword size, Uword alignment) {
start:;
    // TODO: walk the chain and look for free space in all the blocks?
    // That would make better use of the space but might cause bad locality and
    // make allocations slower?
    Uword alignmentSpace = UwordAlignOn((Uword)heap->tail->pos, alignment) - (Uword)heap->tail->pos;
    Uword available = (U8*)heap->tail->end - (U8*)heap->tail->pos;
    if(alignmentSpace + size <= available) {
        heap->tail->pos = (U8*)heap->tail->pos + alignmentSpace;
        Address ret = heap->tail->pos;
        heap->tail->pos = (U8*)heap->tail->pos + size;
        return ret;
    }
    else {
        Uword blockSize = heap->totalSize * 2;
        blockSize = blockSize > size ? blockSize : size;
        HeapBlock block = malloc(sizeof(struct HeapBlock_t) + blockSize);
        if(!block) {
            // TODO: collect garbage here and start over, also better OOM handling ;)
            // Add rootlists to the struct info so that the GC does not have to
            // do a lot of recursive traversals
            assert(False && "Out of memory");
            return NULL;
        }
        block->start = (U8*)block + sizeof(struct HeapBlock_t);
        block->end = (U8*)block->start + blockSize;
        block->pos = block->start;
        block->prev = heap->tail;

        heap->totalSize += blockSize;
        heap->tail = block;

        goto start;
    }
}

static Address OvmHeapAlloc(Heap heap, Type type) {
    // TODO: change the heap->tail->pos parameter if changing how allocation works.
    Uword allocSize = ObjectGetTotalSize(type, False, heap->tail->pos, sizeof(Address));
    Address block = OvmHeapAllocRaw(heap, allocSize, sizeof(Address));
    if(!block) {
        return NULL;
    }
    Address objLocation;
    Address nextLoc;
    ObjectStorageSetup(type, block, False, sizeof(Address), &objLocation, &nextLoc);
    return objLocation;
}

static Array OvmHeapAllocArray(Context ctx, Heap heap, Type elementType, Uword size) {
    Uword alignment = TypeGetAllocationAlignment(elementType);
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    Uword allocSize = metaDataSize;
    allocSize += sizeof(struct Array_t);
    allocSize += alignment;
    allocSize += (TypeGetAllocationSize(elementType) * size);

    Address block = OvmHeapAllocRaw(heap, allocSize, sizeof(Address));
    if(!block) {
        return NULL;
    }

    Object obj = block;
    obj->type = ArrayGetType(ctx);
	Address arrLoc = (U8*)block + metaDataSize;
	Address* backPtrLoc = (Address*)((U8*)arrLoc - sizeof(Address));
    (*backPtrLoc) = block;

    Array arr = arrLoc;
    arr->elementType = elementType;
    arr->size = size;
    arr->data = (Address)UwordAlignOn((Uword)((U8*)arr + sizeof(struct Array_t)), alignment);

    return arr;
}

// Array

static Address ArrayGetFirstElement(Array arr) {
    return arr->data;
}

static Uword ArrayGetSize(Array arr) {
    return arr->size;
}

static Type ArrayGetType(Context ctx) {
    return ContextGetRuntime(ctx)->builtinTypes.referenceTypes.array;
}

static void ArrayPut(Array arr, Uword index, Type elementType, Address src) {
	if (!TypeEquals(arr->elementType, elementType)) {
		assert(False && "This error needs to be handled and set in the context");
	}
	if (index > arr->size - 1) {
		assert(False && "This error needs to be handled and set in the context");
	}
	U8* e = (U8*) ArrayGetFirstElement(arr);
	Uword elementFieldSize = TypeGetFieldSize(elementType);
	e += elementFieldSize * index;
	Address copyFrom;
	if (TypeIsRefType(elementType)) {
		copyFrom = &src;
	}
	else {
		copyFrom = src;
	}
	memcpy(e, copyFrom, elementFieldSize);
}

static void ArrayGet(Array arr, Uword index, Address dest) {
	if (index > arr->size - 1) {
		assert(False && "This error needs to be handled and set in the context");
	}
	U8* e = (U8*) ArrayGetFirstElement(arr);
	Uword elementFieldSize = TypeGetFieldSize(arr->elementType);
	e += elementFieldSize * index;
	memcpy(dest, e, elementFieldSize);
}

static void ArrayCopy(Array from, Uword fromIdx, Array to, Uword toIdx, Uword length) {
	if (!TypeEquals(from->elementType, to->elementType)) {
		assert(False && "This error needs to be handled and set in the context");
	}
	if (fromIdx + length > from->size) {
		assert(False && "This error needs to be handled and set in the context");
	}
	if (toIdx + length > to->size) {
		assert(False && "This error needs to be handled and set in the context");
	}
	Uword elementFieldSize = TypeGetFieldSize(from->elementType);
	U8* fromP = (U8*) ArrayGetFirstElement(from);
	fromP += elementFieldSize * fromIdx;
	U8* toP = (U8*) ArrayGetFirstElement(to);
	toP += elementFieldSize * toIdx;
	memcpy(toP, fromP, elementFieldSize * length);
}

static Type ArrayGetElementType(Array a) {
    return a->elementType;
}

// Context

static Context ContextGetCurrent() {
    return (Context)TLSGet(&currentContext);
}

static Context ContextCreate(Runtime rt, Namespace initialNs) {
    Heap ctxHeap = OvmHeapCreate(1024 * 1024);
    Context ctx = OvmHeapAlloc(ctxHeap, rt->builtinTypes.referenceTypes.context);
    ctx->heap = ctxHeap;
    ctx->runtime = rt;
    ctx->operandStack = OpStackCreate(ctx, ctx->heap);
	ctx->currentNs = initialNs;
    return ctx;
}

static OpStack ContextGetOpStack(Context ctx) {
    return ctx->operandStack;
}

static Runtime ContextGetRuntime(Context ctx) {
    return ctx->runtime;
}

static Heap ContextGetHeap(Context ctx) {
    return ctx->heap;
}

static Namespace ContextGetCurrentNamespace(Context ctx) {
	return ctx->currentNs;
}

static void ContextSetCurrentNamespace(Context ctx, Namespace ns) {
	ctx->currentNs = ns;
}

// StructField

static Type StructFieldGetType(Context ctx) {
    return ContextGetRuntime(ctx)->builtinTypes.valueTypes.structField;
}

static Array StructFieldArrayCreate(Context ctx, Uword size) {
    Heap rtHeap = RuntimeGetHeap(ContextGetRuntime(ctx));
    return OvmHeapAllocArray(ctx, rtHeap, StructFieldGetType(ctx), size);
}

static Type StructFieldGetFieldType(StructField* f) {
    return f->type;
}

// StructInfo

static Type StructInfoGetType(Context ctx) {
    return ContextGetRuntime(ctx)->builtinTypes.referenceTypes.structInfo;
}

static StructInfo StructInfoCreatePrimitive(Context ctx, Uword size, Uword alignment) {
    Runtime rt = ContextGetRuntime(ctx);
    Heap rtHeap = RuntimeGetHeap(rt);
    StructInfo si = OvmHeapAlloc(rtHeap, StructInfoGetType(ctx));
    if(!si) {
        assert(False && "Ouch!"); // TODO: error handling
    }
    si->alignment = alignment;
    si->size = size;
    // TODO: make structFields a Variadic type so that this object allocation can
    // be avoided.
    si->structFields = OvmHeapAllocArray(ctx, rtHeap, rt->builtinTypes.primitiveTypes.nothing, 0);
    if(!si->structFields) {
        assert(False && "Ouch!"); // TODO: error handling
    }
    return si;
}

static StructInfo StructInfoCreate(Context ctx, Array structFields) {
    Runtime rt = ContextGetRuntime(ctx);
    Heap rtHeap = RuntimeGetHeap(rt);
    StructInfo si = OvmHeapAlloc(rtHeap, StructInfoGetType(ctx));
    if(!si) {
        assert(False && "Ouch!"); // TODO: error handling
    }
    Uword arrSize = ArrayGetSize(structFields);
    // TODO: check that the array actually contains structfields, if it has any size.
    StructField* fields = ArrayGetFirstElement(structFields);
    Uword mySize = 0;
    Uword myAlignment = 0;

    for(Uword i = 0; i < arrSize; ++i) {
        Type fieldType = StructFieldGetFieldType(&fields[i]);
        Uword fieldAlignment = TypeGetFieldAlignment(fieldType);
        Uword fieldSize = TypeGetFieldSize(fieldType);
        mySize = UwordAlignOn(mySize, fieldAlignment);
        fields[i].offset = mySize;
        mySize += fieldSize;
        myAlignment = myAlignment > fieldAlignment ? myAlignment : fieldAlignment;
    }

    si->alignment = myAlignment;
    si->size = mySize;
    si->structFields = structFields;
    return si;
}

// Type

static Bool TypeEquals(Type x, Type y) {
	return x.variant == y.variant && x.ref == y.ref;
}

static Uword TypeGetAllocationAlignment(Type type) {
    switch (type.variant) {
        case TYPE_VARIANT_REF:
            return type.ref->structInfo->alignment;
            break;
        case TYPE_VARIANT_VAL:
            return type.val->structInfo->alignment;
            break;
        case TYPE_VARIANT_VAR:
            return type.var->alignment;
            break;
    }
    assert(False && "Unreachable code");
    return 0;
}

static Uword TypeGetFieldAlignment(Type type) {
    switch (type.variant) {
        case TYPE_VARIANT_REF:
            return sizeof(Address);
            break;
        case TYPE_VARIANT_VAL:
            return type.val->structInfo->alignment;
            break;
        case TYPE_VARIANT_VAR:
            return type.var->alignment;
            break;
    }
    assert(False && "Unreachable code");
    return 0;
}

static Uword TypeGetAllocationSize(Type type) {
    switch (type.variant) {
        case TYPE_VARIANT_REF:
            return type.ref->structInfo->size;
            break;
        case TYPE_VARIANT_VAL:
            return type.val->structInfo->size;
            break;
        case TYPE_VARIANT_VAR:
            return type.var->size;
            break;
    }
    assert(False && "Unreachable code");
    return 0;
}

static Uword TypeGetFieldSize(Type type) {
    switch (type.variant) {
        case TYPE_VARIANT_REF:
            return sizeof(Address);
            break;
        case TYPE_VARIANT_VAL:
            return type.val->structInfo->size;
            break;
        case TYPE_VARIANT_VAR:
            return type.var->size;
            break;
    }
    assert(False && "Unreachable code");
    return 0;
}

static Type TypeCreateRefType(Runtime rt, StructInfo si) {
    Type t;
    t.variant = TYPE_VARIANT_REF;
    // TODO: handle OOM
    t.ref = OvmHeapAlloc(RuntimeGetHeap(rt), rt->builtinTypes.referenceTypes.refType);
    t.ref->structInfo = si;
    return t;
}

static Type TypeCreateValType(Runtime rt, StructInfo si) {
    Type t;
    t.variant = TYPE_VARIANT_VAL;
    // TODO: handle OOM
    t.val = OvmHeapAlloc(RuntimeGetHeap(rt), rt->builtinTypes.referenceTypes.valType);
    t.val->structInfo = si;
    return t;
}

static Type TypeCreateVarType(Runtime rt, Array variants) {
    Type t;
    t.variant = TYPE_VARIANT_VAR;
    // TODO: handle OOM
    t.var = OvmHeapAlloc(RuntimeGetHeap(rt), rt->builtinTypes.referenceTypes.varType);
    t.var->variants = variants;
    Uword numValues = ArrayGetSize(variants);
    Value* v = ArrayGetFirstElement(variants);
    Uword size = 0;
    Uword alignment = 0;
    for(Uword i = 0; i < numValues; ++i) {
        Type valType = ObjectGetType(v[i].data);
        Uword tSize = TypeGetFieldSize(valType);
        Uword tAlignment = TypeGetFieldAlignment(valType);
        size = size > tSize ? size : tSize;
        alignment = alignment > tAlignment ? alignment : tAlignment;
    }
    size += sizeof(Uword); // for the variant field
    t.var->alignment = alignment;
    t.var->size = size;
    return t;
}

static Bool TypeIsRefType(Type t) {
	return t.variant == TYPE_VARIANT_REF;
}

static Bool TypeIsValType(Type t) {
	return t.variant == TYPE_VARIANT_VAL;
}

static Bool TypeIsVarType(Type t) {
	return t.variant == TYPE_VARIANT_VAR;
}

// Uword

static Uword UwordAlignOn(Uword offset, Uword on) {
    return (offset + (on - 1)) & (~(on - 1));
}

// Object

static Object ObjectGetMetaData(Address obj) {
	return *((Address*) ((U8*) obj - sizeof(Address)));
}

static Type ObjectGetType(Address obj) {
    return ObjectGetMetaData(obj)->type;
}

static Uword ObjectGetTotalSize(Type type, Bool asField, Address storeLocation, Uword alignmentAfterAllocation) {
    Uword objectAlignment;
    Uword objectSize;
    if(asField) {
        objectAlignment = TypeGetFieldAlignment(type);
        objectSize = TypeGetFieldSize(type);
    }
    else {
        objectAlignment = TypeGetAllocationAlignment(type);
        objectSize = TypeGetAllocationSize(type);
    }
    Uword alignedStoreLocation = UwordAlignOn((Uword)storeLocation, sizeof(Address));
    Uword storeAlignmentDiff = alignedStoreLocation - (Uword)storeLocation;
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    return storeAlignmentDiff + metaDataSize + objectAlignment + objectSize + alignmentAfterAllocation;
}

static void ObjectStorageSetup(Type objType, Address storeLocation, Bool asField, Uword alignmentAfterAllocation, Address* objectLocation, Address* nextStoreLocation) {
    Uword objectAlignment;
    Uword objectSize;
    if(asField) {
        objectAlignment = TypeGetFieldAlignment(objType);
        objectSize = TypeGetFieldSize(objType);
    }
    else {
        objectAlignment = TypeGetAllocationAlignment(objType);
        objectSize = TypeGetAllocationSize(objType);
    }
    // Align store location for storage of metadata
    storeLocation = (Address)UwordAlignOn((Uword)storeLocation, sizeof(Address));
    // Set type and save original allocation pointer to be able to set back pointer.
    Object metaData = (Object)storeLocation;
    metaData->type = objType;
    // Skip past the metadata
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    storeLocation = (Address)UwordAlignOn(((Uword)metaData) + metaDataSize, objectAlignment);
    // Set the back pointer
	Address* backPtrLoc = (Address*)((U8*)storeLocation - sizeof(Address));
    (*backPtrLoc) = metaData;
    // Return address of object
    (*objectLocation) = storeLocation;
    // Return aligned address after allocation
    (*nextStoreLocation) = (Address)UwordAlignOn(((Uword)storeLocation) + objectSize, alignmentAfterAllocation);
}

// Equals

static Equals EqualsCreate(Context ctx, Address self, EqualsFn eqFn) {
    Equals eq;
    eq.eq = eqFn;
    eq.self = self;
    return eq;
}

static Bool EqualsApply(Context ctx, Equals eq, Address other) {
    return eq.eq(ctx, eq.self, other);
}

static FunctionImplementation EqualsFindForType(Context ctx, Type t) {
    Runtime rt = ContextGetRuntime(ctx);
    Vector implVec = rt->builtinProtocols.equals->implementations;
    Uword implCount = VectorGetSize(implVec);
    Array arr = VectorGetBackingArray(implVec);
    ProtocolImplementation* pis = ArrayGetFirstElement(arr);
    for(Uword i = 0; i < implCount; ++i) {
      if(TypeEquals(pis[i]->selfType, t)) {
        Array fns = VectorGetBackingArray(pis[i]->functions);
        FunctionImplementation* fis = ArrayGetFirstElement(fns);
        return fis[0]; // Assume there is a single function
      }
    }
    return NULL; // TODO: return something more sane here. Option type?
}

// Vector

static Vector VectorCreate(Context ctx, Heap heap, Type elementType, Uword initialCapacity) {
	Vector v = (Vector) OvmHeapAlloc(heap, ContextGetRuntime(ctx)->builtinTypes.referenceTypes.vector);
	if (!v) {
		assert(False && "OOM");
	}
	v->data = (Array) OvmHeapAllocArray(ctx, heap, elementType, initialCapacity);
	if (!v->data) {
		assert(False && "OOM");
	}
	v->size = 0;
	return v;
}

static void VectorPush(Context ctx, Heap heap, Vector v, Type valueType, Address valueLocation) {
start:;
	if (v->data->size > v->size) {
		ArrayPut(v->data, v->size, valueType, valueLocation);
		++v->size;
	}
	else {
		Array newData = OvmHeapAllocArray(ctx, heap, v->data->elementType, v->data->size * 2);
		if (!newData) {
			assert(False && "OOM");
		}
		ArrayCopy(v->data, 0, newData, 0, v->data->size);
		v->data = newData;
		goto start;
	}
}

static void VectorPop(Vector v, Address dest) {
	if (v->size > 0) {
		--v->size;
		ArrayGet(v->data, v->size, dest);
	}
}

static Array VectorGetBackingArray(Vector v) {
    return v->data;
}

static Uword VectorGetSize(Vector v) {
    return v->size;
}

static Bool VectorEquals(Context ctx, Vector v1, Vector v2) {
    if(v1 == v2) {
        return True;
    }
    Uword v1Size = VectorGetSize(v1);
    Uword v2Size = VectorGetSize(v2);
    if(v1Size != v2Size) {
        return False;
    }
    // Don't just do an ArrayEquals because we don't care if the backing
    // arrays are not of equal size.
    Array v1Arr = VectorGetBackingArray(v1);
    Array v2Arr = VectorGetBackingArray(v2);
    Type v1ElementType = ArrayGetElementType(v1Arr);
    Type v2ElementType = ArrayGetElementType(v2Arr);
    if(!TypeEquals(v1ElementType, v2ElementType)) {
        return False;
    }
    // Find equals function for type. Error if there is none.
    FunctionImplementation fi = EqualsFindForType(ctx, v1ElementType);
    if(!fi) {
      assert(False && "Type with no equals function. I give up!");
    }
    Bool isNativeFn = FunctionImplementationIsNative(fi);
    U8* v1Ptr = ArrayGetFirstElement(v1Arr);
    U8* v2Ptr = ArrayGetFirstElement(v2Arr);
    Uword elementSize = TypeGetFieldSize(v1ElementType);
    Bool isRefType = TypeIsRefType(v1ElementType);
    for(Uword i = 0; i < v1Size; ++i) {
      U8* actualV1Pointer = v1Ptr + (elementSize * i);
      U8* actualV2Pointer = v2Ptr + (elementSize * i);
      if(isRefType) {
        actualV1Pointer = *((U8**)actualV1Pointer);
        actualV2Pointer = *((U8**)actualV2Pointer);
      }
      Bool result;
      if(isNativeFn) {
        Bool(*fn)(Context ctx, U8* x, U8* y) = FunctionImplementationGetFunctionPointer(fi);
        result = fn(ctx, actualV1Pointer, actualV2Pointer);
      }
      else {
        Runtime rt = ContextGetRuntime(ctx);
        OpStack opStack = ContextGetOpStack(ctx);
        Heap heap = ContextGetHeap(ctx);
        OpStackPush(ctx, heap, opStack, rt->builtinTypes.primitiveTypes.address, actualV2Pointer);
        OpStackPush(ctx, heap, opStack, rt->builtinTypes.primitiveTypes.address, actualV1Pointer);
        octFn fn = (octFn)FunctionImplementationGetFunctionPointer(fi);
        fn(ctx);
        Bool* tmp = (Bool*)OpStackPeek(ctx, opStack, rt->builtinTypes.primitiveTypes.boolean, 0);
        result = *tmp;
      }
      if(!result) {
        return False;
      }
    }
    return True;
}

// Value

static Value ValueCreate(Context ctx, Heap heap, Type t, Address src) {
	Value v;
	// Don't copy ref types.
	// TODO: copy if the destination and source heaps are different.
	if (TypeIsRefType(t)) {
		v.data = src;
	}
	else {
		Runtime rt = ContextGetRuntime(ctx);
		v.data = OvmHeapAlloc(heap, t);
		if (!v.data) {
			// TODO: handle OOM
			return v;
		}
		// TODO: deep copy of object to make sure the whole graph is actually in the correct heap!
		memcpy(v.data, src, TypeGetAllocationSize(t));
	}
	return v;
}

// Namespace

static Namespace NamespaceCreate(Context ctx, String name) {
    Runtime rt = ContextGetRuntime(ctx);
    Heap rtHeap = RuntimeGetHeap(rt);
    Namespace ns = OvmHeapAlloc(rtHeap, rt->builtinTypes.referenceTypes.namespace);
    // TODO: make sure name is in the rt heap
    ns->name = name;
    ns->entries = VectorCreate(ctx, rtHeap, rt->builtinTypes.valueTypes.nameValuePair, 100);
    if(!ns->entries) {
        return NULL;
    }
    return ns;
}

static String NamespaceGetName(Namespace ns) {
    return ns->name;
}

static Value NamespaceBind(Context ctx, Namespace ns, String name, Type t, Address value) {
  NameValuePair entry;
  Runtime rt = ContextGetRuntime(ctx);
  Heap rtHeap = RuntimeGetHeap(rt);
  entry.value = ValueCreate(ctx, rtHeap, t, value);
  entry.name = name; // TODO: make sure the name is in the correct heap!
  // If there is an existing entry with the given name, replace it
  Array entriesArray = VectorGetBackingArray(ns->entries);
  Uword numEntries = VectorGetSize(ns->entries);
  NameValuePair* entries = ArrayGetFirstElement(entriesArray);
  // TODO: Contains function for Vector.
  for(Uword i = 0; i < numEntries; ++i) {
    if(StringEquals(ctx, entries[i].name, entry.name)) {
      entries[i] = entry;
      return entry.value;
    }
  }
  // Not found, just push it.
  VectorPush(ctx, rtHeap, ns->entries, rt->builtinTypes.valueTypes.nameValuePair, &entry);
  return entry.value;
}

static Value NamespaceFind(Context ctx, Namespace ns, String name) {
	Array entriesArray = VectorGetBackingArray(ns->entries);
	Uword numEntries = VectorGetSize(ns->entries);
	NameValuePair* entries = ArrayGetFirstElement(entriesArray);
	for (Uword i = 0; i < numEntries; ++i) {
		if (StringEquals(ctx, entries[i].name, name)) {
			return entries[i].value;
		}
	}
	Value nothing;
	nothing.data = NULL;
	return nothing; // TODO: return nil
}

// String

static String StringCreate(Context ctx, Heap heap, const char* value) {
	Runtime rt = ContextGetRuntime(ctx);
	String s = OvmHeapAlloc(heap, rt->builtinTypes.referenceTypes.string);
    s->size = strlen(value);
	s->utf8Data = OvmHeapAllocArray(ctx, heap, rt->builtinTypes.primitiveTypes.u8, s->size + 1);
    U8* data = ArrayGetFirstElement(s->utf8Data);
    memcpy(data, value, s->size + 1);
    return s;
}

static Bool StringEquals(Context ctx, String s1, String s2) {
	if (s1 == s2) {
		return True;
	}
	if(s1->size != s2->size) {
		return False;
	}
	// TODO: This might crash if the internal format of the data arrays is not the same
	// but it should be the same even when the data is correct utf-8 because the intention
	// is to normalize the data in composed form when creating a string from bytes/c-string.
	return memcmp(s1->utf8Data->data, s2->utf8Data->data, s1->utf8Data->size) == 0;
}

// Function

static FunctionSignature FunctionSignatureCreate(Context ctx, String name, Vector argTypes, Vector retTypes) {
    Runtime rt = ContextGetRuntime(ctx);
    Heap rtHeap = RuntimeGetHeap(rt);
    FunctionSignature sig = OvmHeapAlloc(rtHeap, rt->builtinTypes.referenceTypes.functionSignature);
    sig->arguments = argTypes;
    sig->returns = retTypes;
    sig->name = name;
    // TODO: take these as arguments?
    sig->isPure = False;
    sig->triggersGc = True;
    sig->allocatesMemory = True;
    return sig;
}

//static Bool FunctionSignatureEquals(Context ctx, FunctionSignature sig1, FunctionSignature sig2) {
//  if(sig1 == sig2) {
//    return True;
//  }
//  if(!StringEquals(ctx, sig1->name, sig2->name)) {
//    return False;
//  }
//  sig1->n
//}

// FunctionImplementation

static Bool FunctionImplementationIsNative(FunctionImplementation fi) {
  return fi->isNative;
}

static Address FunctionImplementationGetFunctionPointer(FunctionImplementation fi) {
  return fi->function;
}

// Runtime

static Address RuntimeInitAllocRawObject(Heap heap, Uword size, Uword alignment) {
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    size += alignment;
    size += metaDataSize;

    Address block = OvmHeapAllocRaw(heap, size, sizeof(Address));
    if(!block) {
        return NULL;
    }

    Object obj = block;
    obj->type.variant = 0;
    obj->type.ref = NULL;
	Address ret = (Address) UwordAlignOn((Uword) ((U8*) block + metaDataSize), alignment);
	Address* backPtrLoc = (Address*) ((U8*) ret - sizeof(Address));
    (*backPtrLoc) = block;
    return ret;
}

// This function cannot be used until all the built in types have been allocated
// but they do not have to be initialized.
static Array RuntimeInitAllocRawArray(Runtime rt, Type elementType, Uword elementSize, Uword elementAlignment, Uword arraySize) {
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    Uword allocSize = metaDataSize;
    allocSize += sizeof(struct Array_t);
    allocSize += elementAlignment;
    allocSize += elementSize * arraySize;

    Address block = OvmHeapAllocRaw(RuntimeGetHeap(rt), allocSize, sizeof(Address));
    if(!block) {
        return NULL;
    }

    Object obj = block;
    obj->type = rt->builtinTypes.referenceTypes.array;
	Address arrLoc = (U8*) block + metaDataSize;
	Address* backPtrLoc = (Address*)((U8*) arrLoc - sizeof(Address));
    (*backPtrLoc) = block;

    Array arr = arrLoc;
    arr->elementType = elementType;
    arr->size = arraySize;
    arr->data = (Address)UwordAlignOn((Uword)((U8*)arr + sizeof(struct Array_t)), elementAlignment);

    return arr;
}

static RefType RuntimeInitAllocRefTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct RefType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static ValType RuntimeInitAllocValTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct ValType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static VarType RuntimeInitAllocVarTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct VarType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static StructInfo RuntimeInitAllocStructInfoObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct StructInfo_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.referenceTypes.structInfo;
    return raw;
}

static void RuntimeInitAllocAllPrimitiveTypes(Runtime rt) {
    Type* types = (Type*)&rt->builtinTypes.primitiveTypes;
    Uword numTypes = sizeof(struct BuiltinPrimitiveTypes_t) / sizeof(Type);
    for(Uword i = 0; i < numTypes; ++i) {
        types[i].variant = TYPE_VARIANT_VAL;
        types[i].val = RuntimeInitAllocValTypeObject(rt);
        types[i].val->structInfo = RuntimeInitAllocStructInfoObject(rt);
    }
}

static void RuntimeInitAllocAllVarTypes(Runtime rt) {
    Type* types = (Type*)&rt->builtinTypes.variadicTypes;
    Uword numTypes = sizeof(struct BuiltinVariadicTypes_t) / sizeof(Type);
    for(Uword i = 0; i < numTypes; ++i) {
        // Skip type, it has already been allocated
        if((&types[i]) == (&rt->builtinTypes.variadicTypes.type)) {
            continue;
        }
        types[i].variant = TYPE_VARIANT_VAR;
        types[i].var = RuntimeInitAllocVarTypeObject(rt);
    }
}

static void RuntimeInitAllocAllReferenceTypes(Runtime rt) {
    Type* types = (Type*)&rt->builtinTypes.referenceTypes;
    Uword numTypes = sizeof(struct BuiltinReferenceTypes_t) / sizeof(Type);
    for(Uword i = 0; i < numTypes; ++i) {
        // For structInfo we just need to allocate the structInfo :)
        if((&types[i]) == (&rt->builtinTypes.referenceTypes.structInfo)) {
            types[i].ref->structInfo = RuntimeInitAllocStructInfoObject(rt);
            continue;
        }
        types[i].variant = TYPE_VARIANT_REF;
        types[i].ref = RuntimeInitAllocRefTypeObject(rt);
        types[i].ref->structInfo = RuntimeInitAllocStructInfoObject(rt);
    }
}

static void RuntimeInitAllocAllValueTypes(Runtime rt) {
    Type* types = (Type*)&rt->builtinTypes.valueTypes;
    Uword numTypes = sizeof(struct BuiltinValueTypes_t) / sizeof(Type);
    for(Uword i = 0; i < numTypes; ++i) {
        types[i].variant = TYPE_VARIANT_VAL;
        types[i].val = RuntimeInitAllocValTypeObject(rt);
        types[i].val->structInfo = RuntimeInitAllocStructInfoObject(rt);
    }
}

static void RuntimeInitAllocBuiltInTypes(Runtime rt) {

    // Allocate the types for Type and StructInfo up front so that they
    // can be used in the init functions.

    Heap rtHeap = RuntimeGetHeap(rt);

    rt->builtinTypes.variadicTypes.type.variant = TYPE_VARIANT_VAR;
    Address obj = RuntimeInitAllocRawObject(rtHeap, sizeof(struct VarType_t), sizeof(Address));
    rt->builtinTypes.variadicTypes.type.var = obj;
    Object om = ObjectGetMetaData(obj);
    om->type = rt->builtinTypes.variadicTypes.type; // Type is of type type :)

    rt->builtinTypes.referenceTypes.structInfo.variant = TYPE_VARIANT_REF;
    obj = RuntimeInitAllocRawObject(rtHeap, sizeof(struct RefType_t), sizeof(Address));
    rt->builtinTypes.referenceTypes.structInfo.ref = obj;
    om = ObjectGetMetaData(obj);
    om->type = rt->builtinTypes.variadicTypes.type;

    RuntimeInitAllocAllPrimitiveTypes(rt);
    RuntimeInitAllocAllVarTypes(rt);
    RuntimeInitAllocAllReferenceTypes(rt);
    RuntimeInitAllocAllValueTypes(rt);
}

static void RuntimeInitInitPrimitive(Runtime rt, Type* type, Uword size, Uword alignment) {
    type->val->structInfo->alignment = alignment;
    type->val->structInfo->size = size;
    type->val->structInfo->structFields = RuntimeInitAllocRawArray(rt, rt->builtinTypes.primitiveTypes.nothing, sizeof(struct Nothing_t), sizeof(Address), 0);
}

static void RuntimeInitInitPrimitiveTypes(Runtime rt) {
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.i8, 1, 1);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.u8, 1, 1);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.i16, 2, 2);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.u16, 2, 2);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.i32, 4, 4);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.u32, 4, 4);
#ifdef OCT32
#ifdef __GNUC__
    Uword align64BitTypes = 4;
#else
    Uword align64BitTypes = 8;
#endif
#else
    Uword align64BitTypes = 8;
#endif
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.i64, 8, align64BitTypes);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.u64, 8, align64BitTypes);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.f32, 4, 4);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.f64, 8, align64BitTypes);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.boolean, 1, 1);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.character, 4, 4);
#ifdef OCT64
    Uword wordSize = 8;
#else
    Uword wordSize = 4;
#endif
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.address, wordSize, wordSize);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.nothing, wordSize, wordSize);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.word, wordSize, wordSize);
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.primitiveTypes.uword, wordSize, wordSize);
}

static Array RuntimeInitAllocStructFieldArray(Runtime rt, Uword size) {
    return RuntimeInitAllocRawArray(rt, rt->builtinTypes.valueTypes.structField, sizeof(StructField), sizeof(Address), size);
}

static void RuntimeInitInitStructInfo(Runtime rt, StructInfo si, Uword alignment, Uword size, Uword numFields) {
    si->alignment = alignment;
    si->size = size;
    si->structFields = RuntimeInitAllocStructFieldArray(rt, numFields);
}

static String RuntimeInitCreateString(Runtime rt, const char* str) {
    String s = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct String_t), sizeof(Address));
    Object om = ObjectGetMetaData(s);
    om->type = rt->builtinTypes.referenceTypes.string;
    s->size = strlen(str);
    s->utf8Data = RuntimeInitAllocRawArray(rt, rt->builtinTypes.primitiveTypes.u8, 1, 1, s->size + 1);
    U8* data = ArrayGetFirstElement(s->utf8Data);
    memcpy(data, str, s->size + 1);
    return s;
}

static Type* RuntimeInitAllocTypeObjectOnHeap(Runtime rt, Type copyOf) {
    Type* t = RuntimeInitAllocRawObject(RuntimeGetHeap(rt), sizeof(struct Type_t), sizeof(Address));
    Object om = ObjectGetMetaData(t);
    om->type = rt->builtinTypes.variadicTypes.type;
    (*t) = copyOf;
    return t;
}

static void RuntimeInitInitBuiltInTypes(Context ctx) {
    // The types have now been allocated, but not deeply. They are still missing
    // data members so they cannot be used yet.
    // In order to be able to use the regular heap allocation functions we have
    // to manually initialize all the types that the Type type depends upon.

    Runtime rt = ContextGetRuntime(ctx);
    StructInfo si;
    StructField* sf;
    Uword addrSize = sizeof(Address);

    // Primitives
    RuntimeInitInitPrimitiveTypes(rt);

    // Array
    si = rt->builtinTypes.referenceTypes.array.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct Array_t), 3);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.variadicTypes.type;
    sf[0].offset = offsetof(struct Array_t, elementType);
    sf[0].name = RuntimeInitCreateString(rt, "element-type");
    sf[1].type = rt->builtinTypes.primitiveTypes.uword;
    sf[1].offset = offsetof(struct Array_t, size);
    sf[1].name = RuntimeInitCreateString(rt, "size");
    sf[2].type = rt->builtinTypes.primitiveTypes.address;
    sf[2].offset = offsetof(struct Array_t, data);
    sf[2].name = RuntimeInitCreateString(rt, "data");

    // String
    si = rt->builtinTypes.referenceTypes.string.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct String_t), 2);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.primitiveTypes.uword;
    sf[0].offset = offsetof(struct String_t, size);
    sf[0].name = RuntimeInitCreateString(rt, "size");
    sf[1].type = rt->builtinTypes.referenceTypes.array;
    sf[1].offset = offsetof(struct String_t, utf8Data);
    sf[1].name = RuntimeInitCreateString(rt, "data");

    // StructField
    si = rt->builtinTypes.valueTypes.structField.val->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(StructField), 3);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.variadicTypes.type;
    sf[0].offset = offsetof(StructField, type);
    sf[0].name = RuntimeInitCreateString(rt, "type");
    sf[1].type = rt->builtinTypes.referenceTypes.string;
    sf[1].offset = offsetof(StructField, name);
    sf[1].name = RuntimeInitCreateString(rt, "name");
    sf[2].type = rt->builtinTypes.primitiveTypes.uword;
    sf[2].offset = offsetof(StructField, offset);
    sf[2].name = RuntimeInitCreateString(rt, "offset");

    // StructInfo
    si = rt->builtinTypes.referenceTypes.structInfo.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct StructInfo_t), 3);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.referenceTypes.array;
    sf[0].offset = offsetof(struct StructInfo_t, structFields);
    sf[0].name = RuntimeInitCreateString(rt, "fields");
    sf[1].type = rt->builtinTypes.primitiveTypes.uword;
    sf[1].offset = offsetof(struct StructInfo_t, alignment);
    sf[1].name = RuntimeInitCreateString(rt, "alignment");
    sf[2].type = rt->builtinTypes.primitiveTypes.uword;
    sf[2].offset = offsetof(struct StructInfo_t, size);
    sf[2].name = RuntimeInitCreateString(rt, "size");

    // RefType
    si = rt->builtinTypes.referenceTypes.refType.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct RefType_t), 1);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.referenceTypes.structInfo;
    sf[0].offset = offsetof(struct RefType_t, structInfo);
    sf[0].name = RuntimeInitCreateString(rt, "structure");

    // ValType
    si = rt->builtinTypes.referenceTypes.valType.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct ValType_t), 1);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.referenceTypes.structInfo;
    sf[0].offset = offsetof(struct ValType_t, structInfo);
    sf[0].name = RuntimeInitCreateString(rt, "structure");

    // VarType
    si = rt->builtinTypes.referenceTypes.varType.ref->structInfo;
    RuntimeInitInitStructInfo(rt, si, addrSize, sizeof(struct VarType_t), 3);
    sf = ArrayGetFirstElement(si->structFields);
    sf[0].type = rt->builtinTypes.referenceTypes.array;
    sf[0].offset = offsetof(struct VarType_t, variants);
    sf[0].name = RuntimeInitCreateString(rt, "variants");
    sf[1].type = rt->builtinTypes.primitiveTypes.uword;
    sf[1].offset = offsetof(struct VarType_t, alignment);
    sf[1].name = RuntimeInitCreateString(rt, "alignment");
    sf[2].type = rt->builtinTypes.primitiveTypes.uword;
    sf[2].offset = offsetof(struct VarType_t, size);
    sf[2].name = RuntimeInitCreateString(rt, "size");

    // Type
    rt->builtinTypes.variadicTypes.type.var->size = sizeof(struct Type_t);
    rt->builtinTypes.variadicTypes.type.var->alignment = addrSize;
    rt->builtinTypes.variadicTypes.type.var->variants = RuntimeInitAllocRawArray(rt, rt->builtinTypes.valueTypes.value, sizeof(struct Value_t), addrSize, 3);
    Value* values = ArrayGetFirstElement(rt->builtinTypes.variadicTypes.type.var->variants);
    values[0].data = RuntimeInitAllocTypeObjectOnHeap(rt, rt->builtinTypes.referenceTypes.refType);
    values[1].data = RuntimeInitAllocTypeObjectOnHeap(rt, rt->builtinTypes.referenceTypes.valType);
    values[2].data = RuntimeInitAllocTypeObjectOnHeap(rt, rt->builtinTypes.referenceTypes.varType);

    // Now the regular allocation functions should work so we can use them when
    // defining the rest of the built in types but we still need to take care
    // with the order because they may depend on each other.

    // Value
    Array fields = StructFieldArrayCreate(ctx, 1);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.primitiveTypes.address;
    // Use the RTInit String creation function because it uses the RT heap so
    // there is no need to first create the string and then copy it.
    sf[0].name = RuntimeInitCreateString(rt, "object");
    rt->builtinTypes.valueTypes.value.val->structInfo = StructInfoCreate(ctx, fields);

    // NameValuePair
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.string;
    sf[0].name = RuntimeInitCreateString(rt, "name");
    sf[1].type = rt->builtinTypes.valueTypes.value;
    sf[1].name = RuntimeInitCreateString(rt, "value");
    rt->builtinTypes.valueTypes.nameValuePair.val->structInfo = StructInfoCreate(ctx, fields);

    // Symbol
    fields = StructFieldArrayCreate(ctx, 1);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.string;
    sf[0].name = RuntimeInitCreateString(rt, "name");
    rt->builtinTypes.referenceTypes.symbol.ref->structInfo = StructInfoCreate(ctx, fields);

    // Vector
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.primitiveTypes.uword;
    sf[0].name = RuntimeInitCreateString(rt, "size");
    sf[1].type = rt->builtinTypes.referenceTypes.array;
    sf[1].name = RuntimeInitCreateString(rt, "data");
    rt->builtinTypes.referenceTypes.vector.ref->structInfo = StructInfoCreate(ctx, fields);

    // Object
    fields = StructFieldArrayCreate(ctx, 1);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.variadicTypes.type;
    sf[0].name = RuntimeInitCreateString(rt, "type");
    rt->builtinTypes.referenceTypes.object.ref->structInfo = StructInfoCreate(ctx, fields);

    // Namespace
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.string;
    sf[0].name = RuntimeInitCreateString(rt, "name");
    sf[1].type = rt->builtinTypes.referenceTypes.vector;
    sf[1].name = RuntimeInitCreateString(rt, "entries");
    rt->builtinTypes.referenceTypes.namespace.ref->structInfo = StructInfoCreate(ctx, fields);

    // OpStack
    fields = StructFieldArrayCreate(ctx, 4);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.primitiveTypes.uword;
    sf[0].name = RuntimeInitCreateString(rt, "slot-top");
    sf[1].type = rt->builtinTypes.referenceTypes.array;
    sf[1].name = RuntimeInitCreateString(rt, "slots");
    sf[2].type = rt->builtinTypes.primitiveTypes.address;
    sf[2].name = RuntimeInitCreateString(rt, "data-top");
    sf[3].type = rt->builtinTypes.referenceTypes.array;
    sf[3].name = RuntimeInitCreateString(rt, "data");
    rt->builtinTypes.referenceTypes.opStack.ref->structInfo = StructInfoCreate(ctx, fields);

    // OpStackSlot
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.variadicTypes.type;
    sf[0].name = RuntimeInitCreateString(rt, "type");
    sf[1].type = rt->builtinTypes.primitiveTypes.address;
    sf[1].name = RuntimeInitCreateString(rt, "value");
    rt->builtinTypes.valueTypes.opStackSlot.val->structInfo = StructInfoCreate(ctx, fields);

    // FunctionSignature
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.vector;
    sf[0].name = RuntimeInitCreateString(rt, "argument-types");
    sf[1].type = rt->builtinTypes.referenceTypes.vector;
    sf[1].name = RuntimeInitCreateString(rt, "return-types");
    rt->builtinTypes.referenceTypes.functionSignature.ref->structInfo = StructInfoCreate(ctx, fields);

    // Protocol
    fields = StructFieldArrayCreate(ctx, 3);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.string;
    sf[0].name = RuntimeInitCreateString(rt, "name");
    sf[1].type = rt->builtinTypes.referenceTypes.vector;
    sf[1].name = RuntimeInitCreateString(rt, "placeholders");
    sf[2].type = rt->builtinTypes.referenceTypes.vector;
    sf[2].name = RuntimeInitCreateString(rt, "functions");
    rt->builtinTypes.referenceTypes.protocol.ref->structInfo = StructInfoCreate(ctx, fields);

    // ProtocolFunction
    fields = StructFieldArrayCreate(ctx, 2);
    sf = ArrayGetFirstElement(fields);
    sf[0].type = rt->builtinTypes.referenceTypes.string;
    sf[0].name = RuntimeInitCreateString(rt, "name");
    sf[1].type = rt->builtinTypes.referenceTypes.functionSignature;
    sf[1].name = RuntimeInitCreateString(rt, "signature");
    rt->builtinTypes.valueTypes.protocolFunction.ref->structInfo = StructInfoCreate(ctx, fields);

    // TODO: make proper types for the runtime and context. It is important that all the types
    // used by the ovm are representable in the ovm type system or self-hosting may not be possible.

    // Runtime
    // Make the runtime opaque for now, it is handled specially in the GC anyway.
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.referenceTypes.runtime, sizeof(struct Runtime_t), addrSize);

    // Context
    // Make the context opaque for now, it is handled specially in the GC anyway.
    RuntimeInitInitPrimitive(rt, &rt->builtinTypes.referenceTypes.context, sizeof(struct Context_t), addrSize);
}

static void RuntimeInitCreateBuiltInTypes(Context ctx) {
    // The built in types are circular so we need to allocate memory
    // up front and use some uninitialized pointers to get it going.
    RuntimeInitAllocBuiltInTypes(ContextGetRuntime(ctx));
    RuntimeInitInitBuiltInTypes(ctx);
}

static Namespace RuntimeFindNamespace(Context ctx, String name) {
	Runtime rt = ContextGetRuntime(ctx);
	Array nsArray = VectorGetBackingArray(rt->namespaces);
	Uword size = VectorGetSize(rt->namespaces);
	Namespace* ns = ArrayGetFirstElement(nsArray);
	for (Uword i = 0; i < size; ++i) {
		if (StringEquals(ctx, ns[i]->name, name)) {
			return ns[i];
		}
	}
	return NULL;
}

static void RuntimeAddOrMergeNamespace(Context ctx, Namespace ns) {
	Runtime rt = ContextGetRuntime(ctx);
	Heap rtHeap = RuntimeGetHeap(rt);
	Namespace existing = RuntimeFindNamespace(ctx, NamespaceGetName(ns));
	if (!existing) {
		VectorPush(ctx, rtHeap, rt->namespaces, rt->builtinTypes.referenceTypes.namespace, ns);
	}
	else {
		Array entriesArray = VectorGetBackingArray(ns->entries);
		NameValuePair* newEntries = (NameValuePair*) ArrayGetFirstElement(entriesArray);
		Uword numEntries = VectorGetSize(ns->entries);
		for (Uword i = 0; i < numEntries; ++i) {
			Address val = newEntries[i].value.data;
			Type valueType = ObjectGetType(val);
			NamespaceBind(ctx, existing, newEntries[i].name, valueType, val);
		}
	}
}

static Namespace RuntimeInitCreateOctarineNamespace(Context ctx) {
	Runtime rt = ContextGetRuntime(ctx);
	Heap rtHeap = RuntimeGetHeap(rt);
	String name = StringCreate(ctx, rtHeap, "octarine");
	Namespace octNs = NamespaceCreate(ctx, name);
	RuntimeAddOrMergeNamespace(ctx, octNs);
	ContextSetCurrentNamespace(ctx, octNs);
    return octNs;
}

static void RuntimeInitNSBind(Context ctx, Namespace ns, const char* name, Type type, Address value) {
	Runtime rt = ContextGetRuntime(ctx);
	Heap rtHeap = RuntimeGetHeap(rt);
	NamespaceBind(ctx, ns, StringCreate(ctx, rtHeap, name), type, value);
}

static void RuntimeInitBindBuiltinTypes(Context ctx, Namespace ns) {
	Runtime rt = ContextGetRuntime(ctx);
	Type tt = rt->builtinTypes.variadicTypes.type;

	RuntimeInitNSBind(ctx, ns, "U8", tt, &rt->builtinTypes.primitiveTypes.u8);
	RuntimeInitNSBind(ctx, ns, "I8", tt, &rt->builtinTypes.primitiveTypes.i8);
	RuntimeInitNSBind(ctx, ns, "U16", tt, &rt->builtinTypes.primitiveTypes.u16);
	RuntimeInitNSBind(ctx, ns, "I16", tt, &rt->builtinTypes.primitiveTypes.i16);
	RuntimeInitNSBind(ctx, ns, "U32", tt, &rt->builtinTypes.primitiveTypes.u32);
	RuntimeInitNSBind(ctx, ns, "I32", tt, &rt->builtinTypes.primitiveTypes.i32);
	RuntimeInitNSBind(ctx, ns, "U64", tt, &rt->builtinTypes.primitiveTypes.u64);
	RuntimeInitNSBind(ctx, ns, "I64", tt, &rt->builtinTypes.primitiveTypes.i64);
	RuntimeInitNSBind(ctx, ns, "F32", tt, &rt->builtinTypes.primitiveTypes.f32);
	RuntimeInitNSBind(ctx, ns, "F64", tt, &rt->builtinTypes.primitiveTypes.f64);
	RuntimeInitNSBind(ctx, ns, "Uword", tt, &rt->builtinTypes.primitiveTypes.uword);
	RuntimeInitNSBind(ctx, ns, "Word", tt, &rt->builtinTypes.primitiveTypes.word);
	RuntimeInitNSBind(ctx, ns, "Char", tt, &rt->builtinTypes.primitiveTypes.character);
	RuntimeInitNSBind(ctx, ns, "Bool", tt, &rt->builtinTypes.primitiveTypes.boolean);
	RuntimeInitNSBind(ctx, ns, "Address", tt, &rt->builtinTypes.primitiveTypes.address);
	RuntimeInitNSBind(ctx, ns, "Nothing", tt, &rt->builtinTypes.primitiveTypes.nothing);

    RuntimeInitNSBind(ctx, ns, "NameValuePair", tt, &rt->builtinTypes.valueTypes.nameValuePair);
    RuntimeInitNSBind(ctx, ns, "Value", tt, &rt->builtinTypes.valueTypes.value);
    RuntimeInitNSBind(ctx, ns, "StructField", tt, &rt->builtinTypes.valueTypes.structField);

    RuntimeInitNSBind(ctx, ns, "Type", tt, &rt->builtinTypes.variadicTypes.type);

    RuntimeInitNSBind(ctx, ns, "Array", tt, &rt->builtinTypes.referenceTypes.array);
    RuntimeInitNSBind(ctx, ns, "Context", tt, &rt->builtinTypes.referenceTypes.context);
    RuntimeInitNSBind(ctx, ns, "Namespace", tt, &rt->builtinTypes.referenceTypes.namespace);
    RuntimeInitNSBind(ctx, ns, "ReferenceType", tt, &rt->builtinTypes.referenceTypes.refType);
    RuntimeInitNSBind(ctx, ns, "Runtime", tt, &rt->builtinTypes.referenceTypes.runtime);
    RuntimeInitNSBind(ctx, ns, "String", tt, &rt->builtinTypes.referenceTypes.string);
    RuntimeInitNSBind(ctx, ns, "StructInfo", tt, &rt->builtinTypes.referenceTypes.structInfo);
    RuntimeInitNSBind(ctx, ns, "Symbol", tt, &rt->builtinTypes.referenceTypes.symbol);
    RuntimeInitNSBind(ctx, ns, "ValueType", tt, &rt->builtinTypes.referenceTypes.valType);
    RuntimeInitNSBind(ctx, ns, "VariadicType", tt, &rt->builtinTypes.referenceTypes.varType);
    RuntimeInitNSBind(ctx, ns, "Vector", tt, &rt->builtinTypes.referenceTypes.vector);
}

static void RuntimeInitBindBuiltinFunctions(Context ctx, Namespace ns) {

}

static Runtime RuntimeCreate() {
    Heap rtHeap = OvmHeapCreate(1024 * 1024);
    Runtime rt = RuntimeInitAllocRawObject(rtHeap, sizeof(struct Runtime_t), sizeof(Address));
    rt->heap = rtHeap;
    Heap ctxHeap = OvmHeapCreate(1024 * 1024);
    Context mainCtx = RuntimeInitAllocRawObject(ctxHeap, sizeof(struct Context_t), sizeof(Address));
    mainCtx->runtime = rt;
    mainCtx->heap = ctxHeap;
    RuntimeInitCreateBuiltInTypes(mainCtx);
    Object om = ObjectGetMetaData(rt);
    om->type = rt->builtinTypes.referenceTypes.runtime;

    om = ObjectGetMetaData(mainCtx);
    om->type = rt->builtinTypes.referenceTypes.context;
    mainCtx->operandStack = OpStackCreate(mainCtx, ctxHeap);

    TLSCreate(&currentContext);
    TLSSet(&currentContext, mainCtx);

	rt->namespaces = VectorCreate(mainCtx, rtHeap, rt->builtinTypes.referenceTypes.namespace, 100);
    Namespace octNs = RuntimeInitCreateOctarineNamespace(mainCtx);
	RuntimeInitBindBuiltinTypes(mainCtx, octNs);
	RuntimeInitBindBuiltinFunctions(mainCtx, octNs);

	return rt;
}

static void RuntimeDestroy(Runtime rt) {
    OvmHeapDestroy(RuntimeGetHeap(rt));
    TLSDestroy(&currentContext);
    // TODO: kill all running threads and deallocate their heaps
}

static Heap RuntimeGetHeap(Runtime rt) {
    return rt->heap;
}

// OpStack

static OpStack OpStackCreate(Context ctx, Heap heap) {
    Runtime rt = ContextGetRuntime(ctx);
    OpStack os = OvmHeapAlloc(heap, rt->builtinTypes.referenceTypes.opStack);
    if(!os) {
        assert(False && "OOM");
    }
    os->data = OvmHeapAllocArray(ctx, heap, rt->builtinTypes.primitiveTypes.u8, 10000);
    if(!os->data) {
        assert(False && "OOM");
    }
    os->dataTop = ArrayGetFirstElement(os->data);
    os->slots = OvmHeapAllocArray(ctx, heap, rt->builtinTypes.valueTypes.opStackSlot, 100);
    if(!os->slots) {
        assert(False && "OOM");
    }
    os->slotTop = 0;
    return os;
}

static void OpStackPush(Context ctx, Heap heap, OpStack os, Type valueType, Address src) {
    // Expand slots array if needed
    if(os->slotTop == os->slots->size) {
        Uword newSlotsSize = os->slots->size * 2;
        Array newSlots = OvmHeapAllocArray(ctx, heap, os->slots->elementType, newSlotsSize);
        if(!newSlots) {
            assert(False && "OOM");
        }
        ArrayCopy(os->slots, 0, newSlots, 0, os->slots->size);
        os->slots = newSlots;
    }

    Uword dataSize = TypeGetFieldSize(valueType);
    Uword dataAlignment = TypeGetFieldAlignment(valueType);
    Uword dataAllocationSize = dataSize + dataAlignment;
    Uword dataStackSize = ArrayGetSize(os->data);
    Address startOfDataStack = ArrayGetFirstElement(os->data);
    Address endOfDataStack = (U8*)startOfDataStack + dataStackSize;
    Uword availableDataSpace = (U8*)endOfDataStack - (U8*)os->dataTop;
    // Expand data array if needed
    if(dataAllocationSize > availableDataSpace) {
        Uword newDataSize = dataStackSize * 2;
        newDataSize = newDataSize > dataAllocationSize ? newDataSize : dataAllocationSize + newDataSize;
        Array newDataStack = OvmHeapAllocArray(ctx, heap, os->data->elementType, newDataSize);
        if(!newDataStack) {
            assert(False && "OOM");
        }
        ArrayCopy(os->data, 0, newDataStack, 0, dataStackSize);
        os->data = newDataStack;
        Uword topIdx = (Uword)os->dataTop - (Uword)startOfDataStack;
		os->dataTop = (U8*) ArrayGetFirstElement(os->data) + topIdx;
    }

    // Now there is enough space in both arrays, place value on stack.
    Address dataLocation = (Address)UwordAlignOn((Uword)os->dataTop, dataAlignment);
    memcpy(dataLocation, src, dataSize);

    OpStackSlot* slots = ArrayGetFirstElement(os->slots);
    slots[os->slotTop].type = valueType;
    slots[os->slotTop].value = dataLocation;

    ++os->slotTop;
    os->dataTop = (U8*)dataLocation + dataSize;
}

static Address OpStackPeek(Context ctx, OpStack os, Type expectedType, Uword index) {
    if(index >= os->slotTop) {
        assert(False && "Out of bounds");
    }
    OpStackSlot* slots = ArrayGetFirstElement(os->slots);
    if(!TypeEquals(expectedType, slots[index].type)) {
        assert(False && "Expected and actual types do not match");
    }
    return slots[index].value;
}

static void OpStackPop(Context ctx, OpStack os, Uword numSlots) {
    if(numSlots > os->slotTop) {
        assert(False && "Not enough stack slots to pop; stack corruption?");
    }
    OpStackSlot* slots = ArrayGetFirstElement(os->slots);
    while (numSlots-- > 0) {
        Type slotType = slots[--os->slotTop].type;
        Uword dt = (Uword)os->dataTop;
        dt -= TypeGetFieldSize(slotType);
        os->dataTop = (Address)dt;
    }
    if(os->slotTop == 0) {
        // The code above does not take alignment into account, not sure about
        // the best way to do that so until that works, this will at least make
        // sure that any space lost due to alignment of values is reclaimed when
        // the stack is empty.
        os->dataTop = os->data->data;
    }
}

// VM

// opcodes

#define OP_NOOP  0
#define OP_CALL  1
#define OP_PUSH  2
#define OP_POP   3
#define OP_LOAD  4
#define OP_STORE 5
// probably don't need the halt instruction?
#define OP_HALT  6

static void testAdd(Context ctx) {
    Runtime rt = ContextGetRuntime(ctx);
    Type uwordType = rt->builtinTypes.primitiveTypes.uword;
    Uword* v1;
    Uword* v2;
    OpStack os = ContextGetOpStack(ctx);
    v1 = OpStackPeek(ctx, os, uwordType, 1);
    v2 = OpStackPeek(ctx, os, uwordType, 0);
    Uword result = (*v1) + (*v2);
    OpStackPop(ctx, os, 2);
    OpStackPush(ctx, ContextGetHeap(ctx), os, uwordType, &result);
}

int main(int argc, char* argv[]) {

    Runtime rt = RuntimeCreate();

    U8 program[1000];
    Uword i = 0;
    Type uwordType = rt->builtinTypes.primitiveTypes.uword;
    Type typeType = rt->builtinTypes.variadicTypes.type;
    Type addressType = rt->builtinTypes.primitiveTypes.address;
    Uword uwordSize = TypeGetFieldSize(uwordType);
    Uword typeSize = TypeGetFieldSize(typeType);
    Uword addressSize = TypeGetFieldSize(addressType);

    program[i++] = OP_PUSH;

    Type* t = (Type*)&program[i];
    (*t) = uwordType;
    i += typeSize;

    Uword* uw = (Uword*)&program[i];
    (*uw) = 67;
    i += uwordSize;

    program[i++] = OP_PUSH;

    t = (Type*)&program[i];
    (*t) = uwordType;
    i += typeSize;

    uw = (Uword*)&program[i];
    (*uw) = 3;
    i += uwordSize;

    program[i++] = OP_CALL;

    Address* a = (Address*)&program[i];
    (*a) = testAdd;
    i += addressSize;

    program[i++] = OP_HALT;

    i = 0;

    Context ctx = ContextGetCurrent();
    Heap ctxHeap = ContextGetHeap(ctx);
    OpStack os = ContextGetOpStack(ctx);

    while(True) {
        switch(program[i++]) {
            case OP_NOOP:
            default:
                break;
            case OP_CALL:
                a = (Address*)&program[i];
                octFn fn = *a;
                i += addressSize;
                fn(ctx);
                break;
            case OP_PUSH:
                t = (Type*)&program[i];
                i += typeSize;
                Address valueLocation = &program[i];
                i += addressSize;
                OpStackPush(ctx, ctxHeap, os, *t, valueLocation);
                break;
            case OP_POP:
                uw = (Uword*)&program[i];
                i += uwordSize;
                OpStackPop(ctx, os, *uw);
                break;
            case OP_LOAD:
            case OP_STORE:
            case OP_HALT:
                goto end;
        }
    }
end:;

    uw = OpStackPeek(ctx, os, uwordType, 0);

    printf("Result is: %lu\n", *uw);

    RuntimeDestroy(rt);

    return 0;

}
