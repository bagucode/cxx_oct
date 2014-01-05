
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

struct NamespaceEntry;
typedef struct NamespaceEntry NamespaceEntry;

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

struct Nothing_t;
typedef struct Nothing_t* Nothing;

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
    Array data;
};

struct Object_t {
    Type type;
};

struct NamespaceEntry {
    String key;
    Object value;
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
        Type structField;
        Type namespaceEntry;
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
    } referenceTypes;
};

struct Runtime_t {
    Heap heap;
    BuiltinTypes builtinTypes;
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

struct Context_t {
    Runtime runtime;
    Heap heap;
    Vector operandStack;
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
static Uword UwordAlignOn(Uword x, Uword alignOn);
static Type ArrayGetType(Context ctx);
static Uword ArrayGetSize(Array arr);
static Address ArrayGetFirstElement(Array arr);

// Heap

static Heap HeapCreate(Uword initialSize) {
    const Uword extra = sizeof(struct Heap_t) + sizeof(struct HeapBlock_t);
    Address block = malloc(initialSize + extra);
    if(!block) {
        return NULL;
    }
    Heap heap = (Heap)block;
    heap->tail = block + sizeof(struct Heap_t);
    heap->totalSize = initialSize;
    
    heap->tail->start = heap->tail + sizeof(struct HeapBlock_t);
    heap->tail->end = heap->tail->start + initialSize;
    heap->tail->pos = heap->tail->start;
    heap->tail->prev = NULL;
    
    return heap;
}

static void HeapDestroy(Heap heap) {
    HeapBlock block = heap->tail;
    while(block->prev) {
        HeapBlock prev = block->prev;
        free(block);
        block = prev;
    }
    free(heap);
}

static Address HeapAllocRaw(Heap heap, Uword size, Uword alignment) {
start:;
    // TODO: walk the chain and look for free space in all the blocks?
    // That would make better use of the space but might cause bad locality and
    // make allocations slower?
    Uword alignmentSpace = UwordAlignOn((Uword)heap->tail->pos, alignment) - (Uword)heap->tail->pos;
    Uword available = heap->tail->end - heap->tail->pos;
    if(alignmentSpace + size <= available) {
        heap->tail->pos += alignmentSpace;
        Address ret = heap->tail->pos;
        heap->tail->pos += size;
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
        block->start = block + sizeof(struct HeapBlock_t);
        block->end = block->start + blockSize;
        block->pos = block->start;
        block->prev = heap->tail;
        
        heap->totalSize += blockSize;
        heap->tail = block;
        
        goto start;
    }
}

static Address HeapAlloc(Heap heap, Type type) {
    Uword alignment = TypeGetAllocationAlignment(type);
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    Uword size = metaDataSize;
    size += alignment;
    size += TypeGetAllocationSize(type);
    
    Address block = HeapAllocRaw(heap, size, sizeof(Address));
    if(!block) {
        return NULL;
    }
    
    Object obj = block;
    obj->type = type;
    Address ret = (Address)UwordAlignOn((Uword)(block + metaDataSize), alignment);
    Address* backPtrLoc = ret - sizeof(Address);
    (*backPtrLoc) = block;
    return ret;
}

static Array HeapAllocArray(Context ctx, Heap heap, Type elementType, Uword size) {
    Uword alignment = TypeGetAllocationAlignment(elementType);
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    Uword allocSize = metaDataSize;
    allocSize += sizeof(struct Array_t);
    allocSize += alignment;
    allocSize += (TypeGetAllocationSize(elementType) * size);
    
    Address block = HeapAllocRaw(heap, allocSize, sizeof(Address));
    if(!block) {
        return NULL;
    }
    
    Object obj = block;
    obj->type = ArrayGetType(ctx);
    Address arrLoc = block + metaDataSize;
    Address* backPtrLoc = arrLoc - sizeof(Address);
    (*backPtrLoc) = block;
    
    Array arr = arrLoc;
    arr->elementType = elementType;
    arr->size = size;
    arr->data = (Address)UwordAlignOn((Uword)(arr + sizeof(struct Array_t)), alignment);
    
    return arr;
}

static Heap HeapGetContextHeap(Context ctx) {
    return ctx->heap;
}

static Heap HeapGetRuntimeHeap(Context ctx) {
    return ctx->runtime->heap;
}

// Array

static Address ArrayGetFirstElement(Array arr) {
    return arr->data;
}

// StructField

static Type StructFieldGetType(Context ctx) {
    return ctx->runtime->builtinTypes.valueTypes.structField;
}

static Array StructFieldArrayCreate(Context ctx, Uword size) {
    return HeapAllocArray(ctx, HeapGetRuntimeHeap(ctx), StructFieldGetType(ctx), size);
}

static Context ContextGetCurrent() {
    return (Context)TLSGet(&currentContext);
}

static Type StructFieldGetFieldType(StructField* f) {
    return f->type;
}

// StructInfo

static Type StructInfoGetType(Context ctx) {
    return ctx->runtime->builtinTypes.referenceTypes.structInfo;
}

static StructInfo StructInfoCreatePrimitive(Context ctx, Uword size, Uword alignment) {
    Heap rtHeap = HeapGetRuntimeHeap(ctx);
    StructInfo si = HeapAlloc(rtHeap, StructInfoGetType(ctx));
    if(!si) {
        assert(False && "Ouch!"); // TODO: error handling
    }
    si->alignment = alignment;
    si->size = size;
    // TODO: make structFields a Variadic type so that this object allocation can
    // be avoided.
    si->structFields = HeapAllocArray(ctx, rtHeap, ctx->runtime->builtinTypes.primitiveTypes.nothing, 0);
    if(!si->structFields) {
        assert(False && "Ouch!"); // TODO: error handling
    }
    return si;
}

static StructInfo StructInfoCreate(Context ctx, Array structFields) {
    StructInfo si = HeapAlloc(HeapGetRuntimeHeap(ctx), StructInfoGetType(ctx));
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
        mySize += fieldSize;
        myAlignment = myAlignment > fieldAlignment ? myAlignment : fieldAlignment;
    }
    
    si->alignment = myAlignment;
    si->size = mySize;
    si->structFields = structFields;
    return si;
}

// Array

static Type ArrayGetType(Context ctx) {
    return ctx->runtime->builtinTypes.referenceTypes.array;
}

// Type

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

// Uword

static Uword UwordAlignOn(Uword offset, Uword on) {
    return (offset + (on - 1)) & (~(on - 1));
}

// Object

static Object ObjectGetMetaData(Address obj) {
    return *((Address*)(obj - sizeof(Address)));
}

// Runtime

static Address RuntimeInitAllocRawObject(Heap heap, Uword size, Uword alignment) {
    Uword metaDataSize = sizeof(struct Object_t) + sizeof(Address);
    size += alignment;
    size += metaDataSize;

    Address block = HeapAllocRaw(heap, size, sizeof(Address));
    if(!block) {
        return NULL;
    }
    
    Object obj = block;
    obj->type.variant = 0;
    obj->type.ref = NULL;
    Address ret = (Address)UwordAlignOn((Uword)(block + metaDataSize), alignment);
    Address* backPtrLoc = ret - sizeof(Address);
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
    
    Address block = HeapAllocRaw(rt->heap, allocSize, sizeof(Address));
    if(!block) {
        return NULL;
    }
    
    Object obj = block;
    obj->type = rt->builtinTypes.referenceTypes.array;
    Address arrLoc = block + metaDataSize;
    Address* backPtrLoc = arrLoc - sizeof(Address);
    (*backPtrLoc) = block;
    
    Array arr = arrLoc;
    arr->elementType = elementType;
    arr->size = arraySize;
    arr->data = (Address)UwordAlignOn((Uword)(arr + sizeof(struct Array_t)), elementAlignment);
    
    return arr;
}

static RefType RuntimeInitAllocRefTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(rt->heap, sizeof(struct RefType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static ValType RuntimeInitAllocValTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(rt->heap, sizeof(struct ValType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static VarType RuntimeInitAllocVarTypeObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(rt->heap, sizeof(struct VarType_t), sizeof(Address));
    Object o = ObjectGetMetaData(raw);
    o->type = rt->builtinTypes.variadicTypes.type;
    return raw;
}

static StructInfo RuntimeInitAllocStructInfoObject(Runtime rt) {
    Address raw = RuntimeInitAllocRawObject(rt->heap, sizeof(struct StructInfo_t), sizeof(Address));
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
    
    rt->builtinTypes.variadicTypes.type.variant = TYPE_VARIANT_VAR;
    Address obj = RuntimeInitAllocRawObject(rt->heap, sizeof(struct VarType_t), sizeof(Address));
    rt->builtinTypes.variadicTypes.type.var = obj;
    Object om = ObjectGetMetaData(obj);
    om->type = rt->builtinTypes.variadicTypes.type; // Type is of type type :)

    rt->builtinTypes.referenceTypes.structInfo.variant = TYPE_VARIANT_REF;
    obj = RuntimeInitAllocRawObject(rt->heap, sizeof(struct RefType_t), sizeof(Address));
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
    type->val->structInfo->structFields = RuntimeInitAllocRawArray(rt, rt->builtinTypes.primitiveTypes.nothing, size, alignment, 0);
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
    String s = RuntimeInitAllocRawObject(rt->heap, sizeof(struct String_t), sizeof(Address));
    Object om = ObjectGetMetaData(s);
    om->type = rt->builtinTypes.referenceTypes.string;
    s->size = strlen(str);
    s->utf8Data = RuntimeInitAllocRawArray(rt, rt->builtinTypes.primitiveTypes.u8, 1, 1, s->size + 1);
    U8* data = ArrayGetFirstElement(s->utf8Data);
    memcpy(data, str, s->size + 1);
    return s;
}

static void RuntimeInitInitBuiltInTypes(Runtime rt) {
    // The types have now been allocated, but not deeply. They are still missing
    // data members so they cannot be used yet.
    // In order to be able to use the regular heap allocation functions we have
    // to manually initialize all the types that the Type type depends upon.
    
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

}

static void RuntimeInitCreateBuiltInTypes(Runtime rt) {
    // The built in types are circular so we need to allocate memory
    // up front and use some uninitialized pointers to get it going.
    RuntimeInitAllocBuiltInTypes(rt);
    RuntimeInitInitBuiltInTypes(rt);
}

static Runtime RuntimeCreate() {
    Heap rtHeap = HeapCreate(1024 * 1024);
    Address rtAddr = RuntimeInitAllocRawObject(rtHeap, sizeof(struct Runtime_t), sizeof(Address));
    Runtime rt = rtAddr;
    rt->heap = rtHeap;
    RuntimeInitCreateBuiltInTypes(rt);
    Object om = ObjectGetMetaData(rtAddr);
    om->type = rt->builtinTypes.referenceTypes.runtime;
    
    return rt;
}

static void RuntimeDestroy(Runtime rt) {
    HeapDestroy(rt->heap);
    // TODO: kill all running threads and deallocate their heaps
}

// TEST?

static void opStackPush(Context ctx, Type type, Address src) {
    
}

static void opStackPop(Context ctx, Address dest) {
    
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
    
}

int main(int argc, char* argv[]) {
    
    Runtime rt = RuntimeCreate();

    //Array structFields = StructFieldArrayCreate(&ctx, 10);
    
    printf("Hello World!\n");
    
    RuntimeDestroy(rt);
    
    return 0;
    
}

