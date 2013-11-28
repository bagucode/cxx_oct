#include <iostream>
#include <cassert>

// @ Primitives

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

#ifndef NDEBUG
#define DEBUG
#endif

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

#ifndef NDEBUG
#define DEBUG
#endif

#endif

// @ END primitives

typedef void(*DefaultFn)(void* obj);
typedef void(*DestructorFn)(void* obj);
typedef void(*PrintFn)(void* obj);

struct Type;
struct String;
struct TypeRef;

static Uword getFieldSize(TypeRef* type);
static Uword getFieldAlignment(TypeRef* type);

static Uword align(Uword offset, Uword alignment) {
  return (offset + (alignment - 1)) & (~(alignment - 1));
}

static void* alignPointer(void* ptr, Uword alignment) {
  return (void*)align(Uword(ptr), alignment);
}

static void* alignedMalloc(Uword size, Uword alignment) {
  Uword allocSize = size + alignment - 1 + sizeof(Uword);
  void* place = malloc(allocSize);
  if(!place) {
    throw std::bad_alloc();
  }
  void* alignedPlace = (void*)(((U8*)(place)) + sizeof(Uword));
  alignedPlace = alignPointer(alignedPlace, alignment);
  void** origPlaceStorage = (void**)(Uword(alignedPlace) - sizeof(Uword));
  (*origPlaceStorage) = place;
  return alignedPlace;
}

static Uword calcArraySize(TypeRef* elementType, Uword numElements) {
  Uword allocSize = getFieldSize(elementType);
  Uword elementAlignment = getFieldAlignment(elementType);
  allocSize *= numElements;
  allocSize += sizeof(Uword); // For the array size member
  allocSize += elementAlignment; // For any padding needed between size member and start of values
  return allocSize;
}

static void* alignedArrayMalloc(TypeRef* elementType, Uword numElements) {
  return alignedMalloc(calcArraySize(elementType, numElements), sizeof(Uword));
}

static void alignedFree(void* alignedPlace) {
  void** origPlaceStorage = (void**)(Uword(alignedPlace) - sizeof(Uword));
  free(*origPlaceStorage);
}

static void* getStartOfArrayElements(TypeRef* elementType, void* arr) {
  arr = (void*)(((U8*)(arr)) + sizeof(Uword)); // Skip past size member
  return alignPointer(arr, getFieldAlignment(elementType));
}

// Array. Special type. Accessed through pointers.
template <typename T>
struct Array {
  Uword size;
  T data[];
};

// String, value type.
struct String {
  Uword size; // in unicode codepoints
  Array<U8>* utf8Data;
};

// Function signature information, value type.
struct Function {
  String name;
  Array<TypeRef*>* paramTypes;
  Array<TypeRef*>* returnTypes;
};

// Virtual call dispatch table for Protocol Objects, special type.
// These also contain the type of the object to make runtime conversions between protocol implementations possible.
template <typename PFns>
struct VTable {
  TypeRef* type;
  PFns fns;
};

// Protocol Object. Special type for solving of the expression problem.
// A function may use a Protocol type instead of a concrete type for one or more parameters.
// When it does, any parameter that is of a Protocol type will be automatically converted
// from its concrete representation to a Protocol Object at the call site and then passed to the function.
template <typename T, typename PFns>
struct PObject {
  T* ref;
  VTable<PFns>* vtable;
};

// Field, information about Value Type and Reference Type layout. Value type.
struct Field {
  Uword offset;
  TypeRef* type;
  String name;
};

// Enum with values for the different kinds of types (first-class objects).
enum TypeKind {
  KIND_REF = 0,
  KIND_VAL,
  KIND_ARR,
  KIND_VAR,
  KIND_POB,
  //KIND_FUN
};

struct Type;

// Value Type information. Value Type.
struct ValType {
  // Uword flags? -- immutable etc.
  // Move the functions here to Object protocol?
  PrintFn printFn;
  DefaultFn defaultFn;
  DestructorFn dtorFn;
  Uword size;
  Uword alignment;
  String name;
  Array<Field>* fields;
};

// Protocol Type information. Value Type.
struct ProtocolType {
  String name;
  Array<Function>* functions;
};

// Array Type information. Value Type.
struct ArrType {
  String name;
  TypeRef* elementType;
};

// Type information. Variadic Type.
// This variadic type aggregates all the different type kinds.
struct Type {
  // kind is really TypeKind, not Uword but enum has undefined size.
  // We need it to be machine word size.
  // Theoretically this should work with a fixed-type enum (C++11) but
  // MSVC and GCC have faulty implementations that may pick a different
  // type from the fixed one (C++03 behaviour).
  Uword kind;
  union {
    ValType refValVar;
    ArrType arr;
    ProtocolType pob;
  };
};

// Reference Type wrapper for Type, to make it possible to have pointers
// to type information since Variadic Types are treated like Value Types.
struct TypeRef {
  Type t;
};

struct ObjectProtocolPFns {
  // Nothing here currently.
  // TODO: Move default, dtor and print here?
};

static void* alignedObjectMalloc(TypeRef* type) {
  assert(type->t.kind == KIND_REF && "Attempt to allocate non-reference type on the heap");
  return alignedMalloc(type->t.refValVar.size, type->t.refValVar.alignment);
}

static Bool isPrimitive(TypeRef* type) {
  return type->t.kind == KIND_VAL && type->t.refValVar.fields->size == 0;
}

// Returns the size a given type has at the place of reference when used as a field or in a binding
static Uword getFieldSize(TypeRef* type) {
  switch(type->t.kind) {
  case KIND_REF:
    return sizeof(void*);
  case KIND_VAL:
  case KIND_VAR:
    return type->t.refValVar.size;
  case KIND_ARR:
    return sizeof(void*);
  case KIND_POB:
    return sizeof(void*) * 2;
  default:
    assert(False && "Unrecognized type kind");
  };
}

static Uword getObjectSize(TypeRef* type, void* obj) {
  switch(type->t.kind) {
  case KIND_REF:
  case KIND_VAL:
  case KIND_VAR:
    return type->t.refValVar.size;
  case KIND_ARR:
    return calcArraySize(type->t.arr.elementType, ((Array<U8>*)obj)->size);
  case KIND_POB: {
    PObject<U8, ObjectProtocolPFns>* pob = (PObject<U8, ObjectProtocolPFns>*)obj;
    return getObjectSize(pob->vtable->type, pob->ref);
  } break;
  default:
    assert(False && "Unrecognized type kind");
  };
}

// The alignment to use for the given type when that type is used as a field (taking indirections into account).
static Uword getFieldAlignment(TypeRef* type) {
  switch(type->t.kind) {
  case KIND_VAL:
    return type->t.refValVar.alignment;
  case KIND_VAR: // Contents of the var may have different alignment, but the var itself is aligned on Uword which is same as void*
  case KIND_REF:
  case KIND_ARR:
  case KIND_POB:
    return sizeof(void*);
  default:
    assert(False && "Unrecognized type kind");
  };
}

static Uword getVariant(void* variadic) {
  return *((Uword*)variadic);
}

static void* getVariantAddress(TypeRef* type, void* variadic) {
  return (void*)(Uword(variadic) + type->t.refValVar.fields->data[getVariant(variadic)].offset);
}

static TypeRef* getVariantType(TypeRef* type, void* variadic) {
  return type->t.refValVar.fields->data[getVariant(variadic)].type;
}

static void printTypeName(TypeRef* type, Uword level) {
  char* str = nullptr;
  switch(type->t.kind) {
  case KIND_REF:
  case KIND_VAL:
  case KIND_VAR:
    str = (char*)type->t.refValVar.name.utf8Data->data;
    break;
  case KIND_ARR:
    str = (char*)type->t.arr.name.utf8Data->data;
    break;
  case KIND_POB:
    str = (char*)type->t.pob.name.utf8Data->data;
    break;
  }
  for(Uword i = 0; i < level; ++i) {
    printf("  ");
  }
  printf("%s\n", str);
}

static void printObject(TypeRef* type, void* obj) {
  switch(type->t.kind) {
  case KIND_REF:
  case KIND_VAL:
  case KIND_VAR:
    type->t.refValVar.printFn(obj);
    break;
  case KIND_ARR:
    printf("ARR!");
    break;
  case KIND_POB:
    printf("POB!");
    break;
  }
  printf("\n");
}

// TODO: Make this iterative instead of recursive to not blow the stack when traversing large structures.
static void copy(TypeRef* type, void* src, void* dest) {
  switch(type->t.kind) {
  case KIND_REF: {
    void** srcAsRef = (void**)src;
    void** destAsRef = (void**)dest;
    (*destAsRef) = alignedObjectMalloc(type);
    src = (*srcAsRef);
    dest = (*destAsRef);
  } // no break here, we want to fall through to the VAL code
  case KIND_VAL: {
    if(isPrimitive(type)) {
      memcpy(dest, src, type->t.refValVar.size);
    }
    else {
      for(Uword i = 0; i < type->t.refValVar.fields->size; ++i) {
        U8* srcFieldPtr = ((U8*)src) + type->t.refValVar.fields->data[i].offset;
        U8* destFieldPtr = ((U8*)dest) + type->t.refValVar.fields->data[i].offset;
        if(isPrimitive(type->t.refValVar.fields->data[i].type)) {
          memcpy(destFieldPtr, srcFieldPtr, type->t.refValVar.fields->data[i].type->t.refValVar.size);
        }
        else {
          copy(type->t.refValVar.fields->data[i].type, srcFieldPtr, destFieldPtr);
        }
      }
    }
  } break;
  case KIND_VAR: {
    Uword* srcVariant = (Uword*)src;
    Uword* destVariant = (Uword*)dest;
    (*destVariant) = (*srcVariant);
    TypeRef* variantType = getVariantType(type, src);
    U8* srcVariantPtr = (U8*)getVariantAddress(type, src);
    U8* destVariantPtr = (U8*)getVariantAddress(type, dest);
    copy(variantType, srcVariantPtr, destVariantPtr);
  } break;
  case KIND_ARR: {
    void** srcAsRef = (void**)src;
    void** destAsRef = (void**)dest;
    Array<U8>* srcAsArray = (Array<U8>*)(*srcAsRef);
    (*destAsRef) = alignedArrayMalloc(type->t.arr.elementType, srcAsArray->size);
    Array<U8>* destAsArray = (Array<U8>*)(*destAsRef);
    destAsArray->size = srcAsArray->size;
    U8* srcElementPtr = (U8*)getStartOfArrayElements(type->t.arr.elementType, *srcAsRef);
    U8* destElementPtr = (U8*)getStartOfArrayElements(type->t.arr.elementType, *destAsRef);
    Uword elementSize = getFieldSize(type->t.arr.elementType);
    if(isPrimitive(type->t.arr.elementType)) {
      memcpy(destElementPtr, srcElementPtr, srcAsArray->size * elementSize);
    }
    else {
      for(Uword i = 0; i < srcAsArray->size; ++i) {
        copy(type->t.arr.elementType, srcElementPtr, destElementPtr);
        srcElementPtr += elementSize;
        destElementPtr += elementSize;
      }
    }
  } break;
  case KIND_POB: {
    PObject<U8, ObjectProtocolPFns>* srcAsPObj = (PObject<U8, ObjectProtocolPFns>*)src;
    PObject<U8, ObjectProtocolPFns>* destAsPObj = (PObject<U8, ObjectProtocolPFns>*)dest;
    destAsPObj->vtable = srcAsPObj->vtable;
    if(srcAsPObj->vtable->type->t.kind == KIND_VAL) {
      copy(srcAsPObj->vtable->type, srcAsPObj->ref, destAsPObj->ref);
    } else { // KIND_REF, double indirection expected by copy
      copy(srcAsPObj->vtable->type, &srcAsPObj->ref, &destAsPObj->ref);
    }
  } break;
  default:
    assert(False && "Unrecognized type kind");
  }
}

// TODO: Make this iterative instead of recursive to not blow the stack when traversing large structures.
static void defaultInitObject(TypeRef* type, void* obj) {
  switch(type->t.kind) {
  case KIND_REF:
  case KIND_VAL:
    type->t.refValVar.defaultFn(obj);
    break;
  case KIND_VAR: {
    Uword* objVariant = (Uword*)obj;
    (*objVariant) = 0;
    TypeRef* variantType = getVariantType(type, obj);
    U8* objVariantPtr = (U8*)getVariantAddress(type, obj);
    defaultInitObject(variantType, objVariantPtr);
  } break;
  case KIND_POB: {
    // Should probably be able to default-init protocol objects too.
    // The ref object they contain should be easy to init, the hard part is the vtable.
    // No plan for that yet. Need to store in hashtable in runtime (global?) like in oct3
    assert(False && "Attempt to default-initialize a protocol object (not supported yet)");
  } break;
  case KIND_ARR: {
    void** objAsRef = (void**)obj;
    Array<U8>* objAsArray = (Array<U8>*)(*objAsRef);
    U8* objElementPtr = (U8*)getStartOfArrayElements(type->t.arr.elementType, objAsArray);
    Uword elementSize = getFieldSize(type->t.arr.elementType);
    for(Uword i = 0; i < objAsArray->size; ++i) {
      defaultInitObject(type->t.arr.elementType, objElementPtr);
      objElementPtr += elementSize;
    }
  } break;
  default:
    assert(False && "Unrecognized type kind");
  }
}

// TODO: Make this iterative instead of recursive to not blow the stack when traversing large structures.
static void move(TypeRef* type, void* src, void* dest) {
  switch(type->t.kind) {
  case KIND_REF:
  case KIND_ARR: {
    void** srcAsRef = (void**)src;
    void** destAsRef = (void**)dest;
    (*destAsRef) = (*srcAsRef);
    (*srcAsRef) = nullptr;
  } break;
  case KIND_VAL: {
    memcpy(dest, src, type->t.refValVar.size);
    defaultInitObject(type, src);
  } break;
  case KIND_VAR: {
    Uword* srcVariant = (Uword*)src;
    Uword* destVariant = (Uword*)dest;
    (*destVariant) = (*srcVariant);
    TypeRef* variantType = getVariantType(type, src);
    (*srcVariant) = 0;
    U8* srcVariantPtr = (U8*)getVariantAddress(type, src);
    U8* destVariantPtr = (U8*)getVariantAddress(type, dest);;
    Uword variantAsFieldSize = getFieldSize(variantType);
    memcpy(destVariantPtr, srcVariantPtr, variantAsFieldSize);
    defaultInitObject(variantType, srcVariantPtr);
  } break;
  case KIND_POB: {
    PObject<U8, ObjectProtocolPFns>* srcAsPob = (PObject<U8, ObjectProtocolPFns>*)src;
    PObject<U8, ObjectProtocolPFns>* destAsPob = (PObject<U8, ObjectProtocolPFns>*)dest;
    (*destAsPob) = (*srcAsPob);
    srcAsPob->ref = nullptr;
    srcAsPob->vtable = nullptr;
  } break;
  default:
    assert(False && "Unrecognized type kind");
  }
}

// TODO: Make this iterative instead of recursive to not blow the stack when traversing large structures.
static void destroy(TypeRef* type, void* obj) {
  switch(type->t.kind) {
  case KIND_ARR: {
    void** objAsRef = (void**)obj;
    Array<U8>* objAsArray = (Array<U8>*)(*objAsRef);
    U8* objElementPtr = (U8*)getStartOfArrayElements(type->t.arr.elementType, objAsArray);
    Uword elementSize = getFieldSize(type->t.arr.elementType);
    for(Uword i = 0; i < objAsArray->size; ++i) {
      destroy(type->t.arr.elementType, objElementPtr);
      objElementPtr += elementSize;
    }
    alignedFree(objAsArray);
  } break;
  case KIND_POB: {
    PObject<U8, ObjectProtocolPFns>* objAsPobj = (PObject<U8, ObjectProtocolPFns>*)obj;
    if(objAsPobj->vtable->type->t.kind == KIND_VAL) {
      destroy(objAsPobj->vtable->type, objAsPobj->ref);
    } else { // KIND_REF, double indirection expected by destroy
      destroy(objAsPobj->vtable->type, &objAsPobj->ref);
    }
  } break;
  case KIND_REF: {
    type->t.refValVar.dtorFn(obj);
    void** objAsRef = (void**)obj;
    alignedFree(*objAsRef);
  } break;
  case KIND_VAL: {
    type->t.refValVar.dtorFn(obj);
  } break;
  case KIND_VAR: {
    TypeRef* variantType = getVariantType(type, obj);
    void* variant = getVariantAddress(type, obj);
    destroy(variantType, variant);
  } break;
  }
}

static String* getTypeName(TypeRef* type) {
  switch(type->t.kind) {
  case KIND_ARR:
    return &type->t.arr.name;
  case KIND_POB:
    return &type->t.pob.name;
  case KIND_REF:
  case KIND_VAL:
  case KIND_VAR:
    return &type->t.refValVar.name;
  }
  assert(False && "Unreachable code");
}

static Uword clamp(Uword val, Uword min, Uword max) {
  return val < min ? min : (val > max ? max : val);
}

// Primitives

template <typename T>
static void primitiveDefaultFn(void* obj) {
  memset(obj, 0, sizeof(T));
}

static void primitiveDtorFn(void* obj) {
  // noop
}

template <typename T>
static void primitivePrintFn(void* obj) {
  // No idea now to print it really.
  // Even "primitives" are different.
  // For now, just use this function for all, printing the address and size
  printf("#<Primitive. Size: %lu. Address: 0x%p>", sizeof(T), obj);
}

#define DEFPRIMITIVE(X) \
  static Array<U8> X ## TypeName = { \
    sizeof(#X), \
    #X \
  }; \
  static Array<Field> X ## TypeFields = { \
    0 \
  }; \
  static Type X ## Type = { \
    KIND_VAL, { \
    primitivePrintFn< X >, \
    primitiveDefaultFn< X >, \
    primitiveDtorFn, \
    sizeof( X ), \
    sizeof( X ), { \
    sizeof(#X) - 1, \
    & X ## TypeName \
    }, \
    & X ## TypeFields \
    } \
  }; \
  static TypeRef* X ## TypeRef = (TypeRef*)& X ## Type; \
  static Array<U8> ArrayOf ## X ## TypeName = { \
    sizeof( #X "*"), \
    #X "*" \
  }; \
  static struct { \
    Uword kind; \
    ArrType arr; \
  } ArrayOf ## X ## Type = { \
    KIND_ARR, { \
    { \
    sizeof( #X "*") - 1, \
    &ArrayOf ## X ## TypeName \
    }, \
    X ## TypeRef \
    } \
  }; \
    static TypeRef* ArrayOf ## X ## TypeRef = (TypeRef*)&ArrayOf ## X ## Type
// END DEFPRIMITIVE

    DEFPRIMITIVE(I8);
    DEFPRIMITIVE(U8);
    DEFPRIMITIVE(I16);
    DEFPRIMITIVE(U16);
    DEFPRIMITIVE(I32);
    DEFPRIMITIVE(U32);
    DEFPRIMITIVE(I64);
    DEFPRIMITIVE(U64);
    DEFPRIMITIVE(F32);
    DEFPRIMITIVE(F64);
    DEFPRIMITIVE(Bool);
    DEFPRIMITIVE(Char);
    DEFPRIMITIVE(Word);
    DEFPRIMITIVE(Uword);

    // String definition
    static Array<U8> StringTypeName = {
      sizeof("String"),
      "String"
    };
    static Array<U8> StringSizeFieldName = {
      sizeof("size"),
      "size"
    };
    static Array<U8> StringUtf8DataFieldName = {
      sizeof("utf8-data"),
      "utf8-data"
    };
    static Array<Field> StringTypeFields = {
      2, {
        offsetof(String, size),
        UwordTypeRef,
        {
          sizeof("size") - 1,
          &StringSizeFieldName
        },
        offsetof(String, utf8Data),
        ArrayOfU8TypeRef,
        {
          sizeof("utf8-data") - 1,
          &StringUtf8DataFieldName
        }
      }
    };
    static void StringDefaultFn(void* str_) {
      // TODO: just assign an empty string constant here? Should be equivalent.
      String* str = (String*)str_;
      str->size = 0;
      str->utf8Data = (Array<U8>*)alignedArrayMalloc(U8TypeRef, 0);
      str->utf8Data->size = 0;
    }
    static void StringDtorFn(void* str_) {
      String* str = (String*)str_;
      alignedFree(str->utf8Data);
    }
    static void StringPrintFn(void* str_) {
      String* str = (String*)str_;
      printf("\"%s\"", (const char*)str->utf8Data->data);
    }
    static Type StringType = {
      KIND_VAL, {
        StringPrintFn,
        StringDefaultFn,
        StringDtorFn,
        sizeof(String),
        sizeof(Uword),
        {
          sizeof("String") - 1,
          &StringTypeName
        },
        &StringTypeFields
      }
    };
    static TypeRef* StringTypeRef = (TypeRef*)&StringType;
    static Array<U8> ArrayOfStringTypeName = {
      sizeof("String*"),
      "String*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfStringType = {
      KIND_ARR, {
        {
          sizeof("String*") - 1,
          &ArrayOfStringTypeName
        },
        StringTypeRef
      }
    };
    static TypeRef* ArrayOfStringTypeRef = (TypeRef*)&ArrayOfStringType;
    static String stringFromCString(const char* utf8str) {
      Uword len = strlen(utf8str);
      String str;
      str.size = len;
      str.utf8Data = (Array<U8>*)alignedArrayMalloc(U8TypeRef, len + 1);
      str.utf8Data->size = len + 1;
      U8* arrData = (U8*)getStartOfArrayElements(U8TypeRef, str.utf8Data);
      memcpy(arrData, utf8str, len);
      arrData[len] = '\0';
      return str;
    }
    // End String definition

    // Nothing definition

    struct Nothing {
      Uword dummyValue; // Force Nothing to be word-sized
    };

    DEFPRIMITIVE(Nothing);

    static Array<U8> NothingAsFieldNameCharData = {
      sizeof("nothing"),
      "nothing"
    };

    static String NothingAsFieldName = {
      sizeof("nothing") - 1,
      &NothingAsFieldNameCharData
    };

    // End Nothing definition

    // Self definition
    // Self is a special type that can be used in protocols to make them polymorphic
    // sort of like a template type parameter in C++
    // Since the size must be known at compile time even though the type may not be known
    // when compiling code for protocol objects; protocol objects must still only use reference
    // types in instances, even when using the Self type.

    struct Self {
      Uword dummy; // This is just a marker type. Never actually instantiated. Just give it word size to keep compilers happy.
    };

    DEFPRIMITIVE(Self);

    static Array<U8> SelfAsFieldNameCharData = {
      sizeof("self"),
      "self"
    };

    static String SelfAsFieldName = {
      SelfAsFieldNameCharData.size - 1,
      &SelfAsFieldNameCharData
    };

    // End Self definition

    static TypeRef* getTypeRefTypeRef();
    static TypeRef* getArrayOfTypeRefTypeRef();

    // ArrType definition
    static Array<U8> ArrayTypeTypeName = {
      sizeof("Array"),
      "Array"
    };
    static Array<U8> ArrayTypeNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> ArrayTypeElementTypeFieldName = {
      sizeof("element-type"),
      "element-type"
    };
    static Array<Field> ArrayTypeTypeFields = {
      2, {
        offsetof(ArrType, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &ArrayTypeNameFieldName
        },
        offsetof(ArrType, elementType),
        getTypeRefTypeRef(),
        {
          sizeof("element-type") - 1,
          &ArrayTypeElementTypeFieldName
        }
      }
    };
    static TypeRef* getArrayTypeTypeRef();
    static void ArrayTypeDefaultFn(void* at_) {
      copy(getArrayTypeTypeRef(), &ArrayOfNothingType.arr, at_);
    }
    static void ArrayTypeDtorFn(void* at_) {
      ArrType* at = (ArrType*)at_;
      destroy(StringTypeRef, &at->name);
      destroy(getTypeRefTypeRef(), &at->elementType);
    }
    static void ArrayTypePrintFn(void* at_) {
      ArrType* at = (ArrType*)at_;
      String* elementTypeName = getTypeName(at->elementType);
      printf("#<Array. Element type: %s>", elementTypeName->utf8Data->data);
    }
    static Type ArrayTypeType = {
      KIND_VAL, {
        ArrayTypePrintFn,
        ArrayTypeDefaultFn,
        ArrayTypeDtorFn,
        sizeof(ArrType),
        sizeof(Uword),
        {
          sizeof("Array") - 1,
          &ArrayTypeTypeName
        },
        &ArrayTypeTypeFields
      }
    };
    static TypeRef* ArrayTypeTypeRef = (TypeRef*)&ArrayTypeType;
    static TypeRef* getArrayTypeTypeRef() {
      return ArrayTypeTypeRef;
    }
    static Array<U8> ArrayOfArrayTypeTypeName = {
      sizeof("Array*"),
      "Array*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfArrayTypeType = {
      KIND_ARR, {
        {
          sizeof("Array*") - 1,
          &ArrayOfArrayTypeTypeName
        },
        ArrayTypeTypeRef
      }
    };
    static TypeRef* ArrayOfArrayTypeTypeRef = (TypeRef*)&ArrayOfArrayTypeType;
    // End ArrType definition

    // Field definition
    static Array<U8> FieldTypeName = {
      sizeof("Field"),
      "Field"
    };
    static Array<U8> FieldOffsetFieldName = {
      sizeof("offset"),
      "offset"
    };
    static Array<U8> FieldTypeFieldName = {
      sizeof("type"),
      "type"
    };
    static Array<U8> FieldNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<Field> FieldTypeFields = {
      3, {
        offsetof(Field, offset),
        UwordTypeRef,
        {
          sizeof("offset") - 1,
          &FieldOffsetFieldName
        },
        offsetof(Field, type),
        getTypeRefTypeRef(),
        {
          sizeof("type") - 1,
          &FieldTypeFieldName
        },
        offsetof(Field, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &FieldNameFieldName
        }
      }
    };
    static void FieldDefaultFn(void* f_) {
      Field* f = (Field*)f_;
      f->offset = 0;
      copy(getTypeRefTypeRef(), &NothingTypeRef, &f->type);
      copy(StringTypeRef, &NothingAsFieldName, &f->name);
    }
    static void FieldDtorFn(void* f_) {
      Field* f = (Field*)f_;
      destroy(getTypeRefTypeRef(), &f->type);
      destroy(StringTypeRef, &f->name);
    }
    static void FieldPrintFn(void* f_) {
      Field* f = (Field*)f_;
      printf("#<Field {name: %s, offset: %lu, type: %s}>", f->name.utf8Data->data, f->offset, f->type->t.refValVar.name.utf8Data->data);
    }
    static Type FieldType = {
      KIND_VAL, {
        FieldPrintFn,
        FieldDefaultFn,
        FieldDtorFn,
        sizeof(Field),
        sizeof(Uword), {
          sizeof("Field") - 1,
          &FieldTypeName
        },
        &FieldTypeFields
      }
    };
    static TypeRef* FieldTypeRef = (TypeRef*)&FieldType;
    static Array<U8> ArrayOfFieldTypeName = {
      sizeof("Field*"),
      "Field*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfFieldType = {
      KIND_ARR, {
        {
          sizeof("Field*") - 1,
          &ArrayOfFieldTypeName
        },
        FieldTypeRef
      }
    };
    static TypeRef* ArrayOfFieldTypeRef = (TypeRef*)&ArrayOfFieldType;
    // End Field definition

    // Object protocol definition
    static Array<U8> ObjectProtocolName = {
      sizeof("Object"),
      "Object"
    };
    static Array<Function> ObjectProtocolFunctions = { 0 }; // TODO: Move defaultFn and dtorFn from type info to object protocol?
    static struct {
      Uword kind;
      ProtocolType pt;
    } ObjectProtocolType = {
      KIND_POB, {
        {
          sizeof("Object") - 1,
          &ObjectProtocolName
        },
        &ObjectProtocolFunctions
      }
    };
    static TypeRef* ObjectProtocolTypeRef = (TypeRef*)&ObjectProtocolType;
    // End Object protocol definition

    // Function definition
    static Array<U8> FunctionTypeName = {
      sizeof("Function"),
      "Function"
    };
    static Array<U8> FunctionNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> FunctionParamTypesFieldName = {
      sizeof("param-types"),
      "param-types"
    };
    static Array<U8> FunctionReturnTypesFieldName = {
      sizeof("return-types"),
      "return-types"
    };
    static Array<Field> FunctionTypeFields = {
      3, {
        offsetof(Function, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &FunctionNameFieldName
        },
        offsetof(Function, paramTypes),
        getArrayOfTypeRefTypeRef(),
        {
          sizeof("param-types") - 1,
          &FunctionParamTypesFieldName
        },
        offsetof(Function, returnTypes),
        getArrayOfTypeRefTypeRef(),
        {
          sizeof("return-types") - 1,
          &FunctionReturnTypesFieldName
        }
      }
    };
    static Array<U8> IdentityFunctionName = {
      sizeof("identity"),
      "identity"
    };
    static Array<TypeRef*> IdentityFunctionParamTypes = {
      1,
      ObjectProtocolTypeRef
    };
    static Array<TypeRef*> IdentityFunctionReturnTypes = {
      1,
      ObjectProtocolTypeRef
    };
    static Function IdentityFunction = {
      {
        sizeof("identity") - 1,
        &IdentityFunctionName
      },
      &IdentityFunctionParamTypes,
      &IdentityFunctionReturnTypes
    };
    static TypeRef* getFunctionTypeRef();
    static void FunctionDefaultFn(void* f_) {
      /*

        Default signature should be that of the identity function:

        (defn identity [Object x] [Object]
        x)

        First vector contains pairs of type and name for parameters, second vector
        contains types for return values.

        Generic type parameters are not needed, protocols can be used instead (like Object).
        If a protocol is used as the type for a parameter and the function source
        is available then the compiler can generate a specific function for whatever
        type happens to be used in the protocol parameter slot at a call site.
        This should be the default behaviour when the compiler has the source for
        the called function. A generic version of the function that uses protocol
        objects instead will also be generated by the compiler and used when calling
        the function from other modules (if we assume that the code will not be available
        in that case, perhaps it could be).

      */
      Function* f = (Function*)f_;
      copy(getFunctionTypeRef(), &IdentityFunction, &f);
    }
    static void FunctionDtorFn(void* f_) {
      Function* f = (Function*)f_;
      destroy(StringTypeRef, &f->name);
      destroy(getArrayOfTypeRefTypeRef(), &f->paramTypes);
      destroy(getArrayOfTypeRefTypeRef(), &f->returnTypes);
    }
    static void FunctionPrintFn(void* f_) {
      Function* f = (Function*)f_;
      printf("#<Function {name: %s, paramTypes: [", f->name.utf8Data->data);
      for(Uword i = 0; i < f->paramTypes->size; ++i) {
        if(i > 0) {
          printf(", ");
        }
        U8* name;
        switch(f->paramTypes->data[i]->t.kind) {
        case KIND_ARR:
          name = f->paramTypes->data[i]->t.arr.name.utf8Data->data;
          break;
        case KIND_POB:
          name = f->paramTypes->data[i]->t.pob.name.utf8Data->data;
          break;
        case KIND_REF:
        case KIND_VAL:
        case KIND_VAR:
          name = f->paramTypes->data[i]->t.refValVar.name.utf8Data->data;
          break;
        }
        printf("%s", name);
      }
      printf("], returnTypes: [");
      for(Uword i = 0; i < f->returnTypes->size; ++i) {
        if(i > 0) {
          printf(", ");
        }
        U8* name;
        switch(f->returnTypes->data[i]->t.kind) {
        case KIND_ARR:
          name = f->returnTypes->data[i]->t.arr.name.utf8Data->data;
          break;
        case KIND_POB:
          name = f->returnTypes->data[i]->t.pob.name.utf8Data->data;
          break;
        case KIND_REF:
        case KIND_VAL:
        case KIND_VAR:
          name = f->returnTypes->data[i]->t.refValVar.name.utf8Data->data;
          break;
        }
        printf("%s", name);
      }
      printf("]}>");
    }
    static Type FunctionType = {
      KIND_VAL, {
        FunctionPrintFn,
        FunctionDefaultFn,
        FunctionDtorFn,
        sizeof(Function),
        sizeof(Uword), {
          sizeof("Function") - 1,
          &FunctionTypeName
        },
        &FunctionTypeFields
      }
    };
    static TypeRef* FunctionTypeRef = (TypeRef*)&FunctionType;
    static TypeRef* getFunctionTypeRef() {
      return FunctionTypeRef;
    }
    static Array<U8> ArrayOfFunctionTypeName = {
      sizeof("Function*"),
      "Function*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfFunctionType = {
      KIND_ARR, {
        {
          sizeof("Function*") - 1,
          &ArrayOfFunctionTypeName
        },
        FunctionTypeRef
      }
    };
    static TypeRef* ArrayOfFunctionTypeRef = (TypeRef*)&ArrayOfFunctionType;
    // End Function definition

    // Address definition. Opaque type used to represent raw pointers
    struct Address {
      void* ptr;
    };
    DEFPRIMITIVE(Address);
    // End Address definition.

    // ValType definition
    static Array<U8> ValTypeTypeName = {
      sizeof("ValType"),
      "ValType"
    };
    static Array<U8> ValTypePrintFnFieldName = {
      sizeof("print-fn"),
      "print-fn"
    };
    static Array<U8> ValTypeDefaultFnFieldName = {
      sizeof("default-fn"),
      "default-fn"
    };
    static Array<U8> ValTypeDtorFnFieldName = {
      sizeof("dtor-fn"),
      "dtor-fn"
    };
    static Array<U8> ValTypeSizeFieldName = {
      sizeof("size"),
      "size"
    };
    static Array<U8> ValTypeAlignmentFieldName = {
      sizeof("alignment"),
      "alignment"
    };
    static Array<U8> ValTypeNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> ValTypeFieldsFieldName = {
      sizeof("fields"),
      "fields"
    };
    static Array<Field> ValTypeTypeFields = {
      7, {
        offsetof(ValType, printFn),
        AddressTypeRef,
        {
          sizeof("print-fn") - 1,
          &ValTypePrintFnFieldName
        },
        offsetof(ValType, defaultFn),
        AddressTypeRef,
        {
          sizeof("default-fn") - 1,
          &ValTypeDefaultFnFieldName
        },
        offsetof(ValType, dtorFn),
        AddressTypeRef,
        {
          sizeof("dtor-fn") - 1,
          &ValTypeDtorFnFieldName
        },
        offsetof(ValType, size),
        UwordTypeRef,
        {
          sizeof("size") - 1,
          &ValTypeSizeFieldName
        },
        offsetof(ValType, alignment),
        UwordTypeRef,
        {
          sizeof("alignment") - 1,
          &ValTypeAlignmentFieldName
        },
        offsetof(ValType, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &ValTypeNameFieldName
        },
        offsetof(ValType, fields),
        ArrayOfFieldTypeRef,
        {
          sizeof("fields") - 1,
          &ValTypeFieldsFieldName
        }
      }
    };
    static TypeRef* getValTypeTypeRef();
    static void ValTypeDefaultFn(void* vt_) {
      copy(getValTypeTypeRef(), &UwordTypeRef->t.refValVar, vt_);
    }
    static void ValTypeDtorFn(void* vt_) {
      ValType* self = (ValType*)vt_;
      destroy(StringTypeRef, &self->name);
      destroy(ArrayOfFieldTypeRef, &self->fields);
    }
    static void ValTypePrintFn(void* vt_) {
      ValType* self = (ValType*)vt_;
      printf("#<ValType {alignment: %lu, default: 0x%p, dtor: 0x%p, name: %s, print: 0x%p, size: %lu, fields: [", self->alignment, self->defaultFn, self->dtorFn, self->name.utf8Data->data, self->printFn, self->size);
      for(Uword i = 0; i < self->fields->size; ++i) {
        if(i > 0) {
          printf(", ");
        }
        U8* name = self->fields->data[i].name.utf8Data->data;
        U8* type;
        switch(self->fields->data[i].type->t.kind) {
        case KIND_ARR:
          type = self->fields->data[i].type->t.arr.name.utf8Data->data;
          break;
        case KIND_POB:
          type = self->fields->data[i].type->t.pob.name.utf8Data->data;
          break;
        case KIND_REF:
        case KIND_VAL:
        case KIND_VAR:
          type = self->fields->data[i].type->t.refValVar.name.utf8Data->data;
          break;
        }
        printf("{name: %s, type: %s}", name, type);
      }
      printf("]}>");
    }
    static Type ValTypeType = {
      KIND_VAL, {
        ValTypePrintFn,
        ValTypeDefaultFn,
        ValTypeDtorFn,
        sizeof(ValType),
        sizeof(Uword),
        {
          sizeof("ValType") - 1,
          &ValTypeTypeName
        },
        &ValTypeTypeFields
      }
    };
    static TypeRef* ValTypeTypeRef = (TypeRef*)&ValTypeType;
    static TypeRef* getValTypeTypeRef() {
      return ValTypeTypeRef;
    }
    static Array<U8> ArrayOfValTypeTypeName = {
      sizeof("ValType*"),
      "ValType*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfValTypeType = {
      KIND_ARR, {
        {
          sizeof("ValType*") - 1,
          &ArrayOfValTypeTypeName
        },
        ValTypeTypeRef
      }
    };
    static TypeRef* ArrayOfValTypeTypeRef = (TypeRef*)&ArrayOfValTypeType;
    // End ValType definition

    // RefType definition (identical to ValType)
    static Array<U8> RefTypeTypeName = {
      sizeof("RefType"),
      "RefType"
    };
    static Array<U8> RefTypePrintFnFieldName = {
      sizeof("print-fn"),
      "print-fn"
    };
    static Array<U8> RefTypeDefaultFnFieldName = {
      sizeof("default-fn"),
      "default-fn"
    };
    static Array<U8> RefTypeDtorFnFieldName = {
      sizeof("dtor-fn"),
      "dtor-fn"
    };
    static Array<U8> RefTypeSizeFieldName = {
      sizeof("size"),
      "size"
    };
    static Array<U8> RefTypeAlignmentFieldName = {
      sizeof("alignment"),
      "alignment"
    };
    static Array<U8> RefTypeNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> RefTypeFieldsFieldName = {
      sizeof("fields"),
      "fields"
    };
    static Array<Field> RefTypeTypeFields = {
      7, {
        offsetof(ValType, printFn),
        AddressTypeRef,
        {
          sizeof("print-fn") - 1,
          &RefTypePrintFnFieldName
        },
        offsetof(ValType, defaultFn),
        AddressTypeRef,
        {
          sizeof("default-fn") - 1,
          &RefTypeDefaultFnFieldName
        },
        offsetof(ValType, dtorFn),
        AddressTypeRef,
        {
          sizeof("dtor-fn") - 1,
          &RefTypeDtorFnFieldName
        },
        offsetof(ValType, size),
        UwordTypeRef,
        {
          sizeof("size") - 1,
          &RefTypeSizeFieldName
        },
        offsetof(ValType, alignment),
        UwordTypeRef,
        {
          sizeof("alignment") - 1,
          &RefTypeAlignmentFieldName
        },
        offsetof(ValType, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &RefTypeNameFieldName
        },
        offsetof(ValType, fields),
        ArrayOfFieldTypeRef,
        {
          sizeof("fields") - 1,
          &RefTypeFieldsFieldName
        }
      }
    };
    static TypeRef* getRefTypeTypeRef();
    static void RefTypeDefaultFn(void* vt_) {
      copy(getRefTypeTypeRef(), &UwordTypeRef->t.refValVar, vt_);
    }
    static void RefTypeDtorFn(void* vt_) {
      ValType* self = (ValType*)vt_;
      destroy(StringTypeRef, &self->name);
      destroy(ArrayOfFieldTypeRef, &self->fields);
    }
    static Type RefTypeType = {
      KIND_VAL, {
        ValTypePrintFn,
        RefTypeDefaultFn,
        RefTypeDtorFn,
        sizeof(ValType),
        sizeof(Uword),
        {
          sizeof("RefType") - 1,
          &RefTypeTypeName
        },
        &RefTypeTypeFields
      }
    };
    static TypeRef* RefTypeTypeRef = (TypeRef*)&RefTypeType;
    static TypeRef* getRefTypeTypeRef() {
      return RefTypeTypeRef;
    }
    static Array<U8> ArrayOfRefTypeTypeName = {
      sizeof("RefType*"),
      "RefType*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfRefTypeType = {
      KIND_ARR, {
        {
          sizeof("RefType*") - 1,
          &ArrayOfRefTypeTypeName
        },
        RefTypeTypeRef
      }
    };
    static TypeRef* ArrayOfRefTypeTypeRef = (TypeRef*)&ArrayOfRefTypeType;
    // End RefType definition

    // ProtocolType definition
    static Array<U8> ProtocolTypeTypeName = {
      sizeof("Protocol"),
      "Protocol"
    };
    static Array<U8> ProtocolTypeNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> ProtocolTypeFunctionsFieldName = {
      sizeof("functions"),
      "functions"
    };
    static Array<Field> ProtocolTypeFields = {
      2, {
        offsetof(ProtocolType, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &ProtocolTypeNameFieldName
        },
        offsetof(ProtocolType, functions),
        ArrayOfFunctionTypeRef,
        {
          sizeof("functions") - 1,
          &ProtocolTypeFunctionsFieldName
        }
      }
    };
    static TypeRef* getProtocolTypeTypeRef();
    static void ProtocolTypeDefaultFn(void* pt_) {
      copy(getProtocolTypeTypeRef(), &ObjectProtocolType, pt_);
    }
    static void ProtocolTypeDtorFn(void* pt_) {
      ProtocolType* self = (ProtocolType*)pt_;
      destroy(StringTypeRef, &self->name);
      destroy(ArrayOfFunctionTypeRef, &self->functions);
    }
    static void ProtocolPrintFn(void* pt_) {
      ProtocolType* self = (ProtocolType*)pt_;
      printf("#<Protocol {name: %s}>", self->name.utf8Data->data);
    }
    static Type ProtocolTypeType = {
      KIND_VAL, {
        ProtocolPrintFn,
        ProtocolTypeDefaultFn,
        ProtocolTypeDtorFn,
        sizeof(ProtocolType),
        sizeof(Uword),
        {
          sizeof("Protocol") - 1,
          &ProtocolTypeTypeName
        },
        &ProtocolTypeFields
      }
    };
    static TypeRef* ProtocolTypeTypeRef = (TypeRef*)&ProtocolTypeType;
    static TypeRef* getProtocolTypeTypeRef() {
      return ProtocolTypeTypeRef;
    }
    static Array<U8> ArrayOfProtocolTypeTypeName = {
      sizeof("Protocol*"),
      "Protocol*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfProtocolTypeType = {
      KIND_ARR, {
        {
          sizeof("Protocol*") - 1,
          &ArrayOfProtocolTypeTypeName
        },
        ProtocolTypeTypeRef
      }
    };
    static TypeRef* ArrayOfProtocolTypeTypeRef = (TypeRef*)&ArrayOfProtocolTypeType;
    // End ProtocolType definition

    // ObjectOption definition
    enum OptionVariants {
      OBJECT_OPTION_NOTHING = 0,
      OBJECT_OPTION_OBJECT
    };
    struct ObjectOption {
      Uword variant;
      union {
        Nothing nothing;
        PObject<Self, ObjectProtocolPFns> object;
      };
    };
    static Array<U8> ObjectOptionTypeName = {
      sizeof("ObjectOption"),
      "ObjectOption"
    };
    static Array<U8> ObjectOptionNothingFieldName = {
      sizeof("nothing"),
      "nothing"
    };
    static Array<U8> ObjectOptionObjectFieldName = {
      sizeof("object"),
      "object"
    };
    static Array<Field> ObjectOptionFields = {
      2, {
        offsetof(ObjectOption, nothing),
        NothingTypeRef,
        {
          sizeof("nothing") - 1,
          &ObjectOptionNothingFieldName
        },
        offsetof(ObjectOption, object),
        ObjectProtocolTypeRef,
        {
          sizeof("object") - 1,
          &ObjectOptionObjectFieldName
        }
      }
    };
    static void ObjectOptionDefaultFn(void* opt_) {
      ObjectOption* self = (ObjectOption*)opt_;
      self->variant = OBJECT_OPTION_NOTHING;
      self->nothing.dummyValue = 0;
    }
    static void ObjectOptionDtorFn(void* opt_) {
      ObjectOption* self = (ObjectOption*)opt_;
      if(self->variant == OBJECT_OPTION_OBJECT) {
        if(self->object.vtable->type->t.kind == KIND_VAL) {
          // TODO: remove this branch? Only ref types should be in protocol objects
          destroy(self->object.vtable->type, self->object.ref);
        } else { // KIND_REF, double indirection expected by destroy
          destroy(self->object.vtable->type, &self->object.ref);
        }
      }
    }
    static void ObjectOptionPrintFn(void* opt_) {
      ObjectOption* self = (ObjectOption*)opt_;
      printf("#<ObjectOption {??}>");
    }
    static Type ObjectOptionType = {
      KIND_VAR, {
        ObjectOptionPrintFn,
        ObjectOptionDefaultFn,
        ObjectOptionDtorFn,
        sizeof(ObjectOption),
        sizeof(Uword),
        {
          sizeof("ObjectOption") - 1,
          &ObjectOptionTypeName
        },
        &ObjectOptionFields
      }
    };
    static TypeRef* ObjectOptionTypeRef = (TypeRef*)&ObjectOptionType;
    static Array<U8> ArrayOfObjectOptionTypeName = {
      sizeof("ObjectOption*"),
      "ObjectOption*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfObjectOptionType = {
      KIND_ARR, {
        {
          sizeof("ObjectOption*") - 1,
          &ArrayOfObjectOptionTypeName
        },
        ObjectOptionTypeRef
      }
    };
    static TypeRef* ArrayOfObjectOptionTypeRef = (TypeRef*)&ArrayOfObjectOptionType;
    // End ObjectOption definition

    // VarType definition
    static Array<U8> VarTypeTypeName = {
      sizeof("Variadic"),
      "Variadic"
    };
    static Array<U8> VarTypePrintFnFieldName = {
      sizeof("print-fn"),
      "print-fn"
    };
    static Array<U8> VarTypeDefaultFnFieldName = {
      sizeof("default-fn"),
      "default-fn"
    };
    static Array<U8> VarTypeDtorFnFieldName = {
      sizeof("dtor-fn"),
      "dtor-fn"
    };
    static Array<U8> VarTypeSizeFieldName = {
      sizeof("size"),
      "size"
    };
    static Array<U8> VarTypeAlignmentFieldName = {
      sizeof("alignment"),
      "alignment"
    };
    static Array<U8> VarTypeNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<U8> VarTypeFieldsFieldName = {
      sizeof("fields"),
      "fields"
    };
    static Array<Field> VarTypeTypeFields = {
      7, {
        offsetof(ValType, printFn),
        AddressTypeRef,
        {
          sizeof("print-fn") - 1,
          &VarTypePrintFnFieldName
        },
        offsetof(ValType, dtorFn),
        AddressTypeRef,
        {
          sizeof("dtor-fn") - 1,
          &VarTypeDtorFnFieldName
        },
        offsetof(ValType, size),
        UwordTypeRef,
        {
          sizeof("size") - 1,
          &VarTypeSizeFieldName
        },
        offsetof(ValType, alignment),
        UwordTypeRef,
        {
          sizeof("alignment") - 1,
          &VarTypeAlignmentFieldName
        },
        offsetof(ValType, name),
        StringTypeRef,
        {
          sizeof("name") - 1,
          &VarTypeNameFieldName
        },
        offsetof(ValType, fields),
        ArrayOfFieldTypeRef,
        {
          sizeof("fields") - 1,
          &VarTypeFieldsFieldName
        }
      }
    };
    static TypeRef* getVarTypeTypeRef();
    static void VarTypeDefaultFn(void* vt_) {
      copy(getVarTypeTypeRef(), &ObjectOptionTypeRef->t.refValVar, vt_);
    }
    static void VarTypeDtorFn(void* vt_) {
      ValType* self = (ValType*)vt_;
      destroy(StringTypeRef, &self->name);
      destroy(ArrayOfFieldTypeRef, &self->fields);
    }
    static Type VarTypeType = {
      KIND_VAL, {
        ValTypePrintFn,
        VarTypeDefaultFn,
        VarTypeDtorFn,
        sizeof(ValType),
        sizeof(Uword),
        {
          sizeof("Variadic") - 1,
          &VarTypeTypeName
        },
        &VarTypeTypeFields
      }
    };
    static TypeRef* VarTypeTypeRef = (TypeRef*)&VarTypeType;
    static TypeRef* getVarTypeTypeRef() {
      return VarTypeTypeRef;
    }
    static Array<U8> ArrayOfVarTypeTypeName = {
      sizeof("Variadic*"),
      "Variadic*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfVarTypeType = {
      KIND_ARR, {
        {
          sizeof("Variadic*") - 1,
          &ArrayOfVarTypeTypeName
        },
        VarTypeTypeRef
      }
    };
    static TypeRef* ArrayOfVarTypeTypeRef = (TypeRef*)&ArrayOfVarTypeType;
    // End VarType definition

    // Type definition
    static Array<U8> TypeTypeName = {
      sizeof("Type"),
      "Type"
    };
    static Array<U8> TypeValFieldName = {
      sizeof("value"),
      "value"
    };
    static Array<U8> TypeRefFieldName = {
      sizeof("reference"),
      "reference"
    };
    static Array<U8> TypeVarFieldName = {
      sizeof("variadic"),
      "variadic"
    };
    static Array<U8> TypeArrFieldName = {
      sizeof("array"),
      "array"
    };
    static Array<U8> TypePobFieldName = {
      sizeof("protocol"),
      "protocol"
    };
    static Array<Field> TypeFields = {
      5, {
        offsetof(Type, refValVar),
        RefTypeTypeRef,
        {
          sizeof("reference") - 1,
          &TypeRefFieldName
        },
        offsetof(Type, refValVar),
        ValTypeTypeRef,
        {
          sizeof("value") - 1,
          &TypeValFieldName
        },
        offsetof(Type, arr),
        ArrayTypeTypeRef,
        {
          sizeof("array") - 1,
          &TypeArrFieldName
        },
        offsetof(Type, refValVar),
        VarTypeTypeRef,
        {
          sizeof("variadic") - 1,
          &TypeVarFieldName
        },
        offsetof(Type, pob),
        ProtocolTypeTypeRef,
        {
          sizeof("protocol") - 1,
          &TypePobFieldName
        }
      }
    };
    static TypeRef* getTypeTypeRef();
    static void TypeDefaultFn(void* t_) {
      Type* self = (Type*)t_;
      TypeRef* tt = getTypeTypeRef();
      copy(tt, &tt->t, self);
    }
    static void TypeDtorFn(void* t_) {
      TypeRef* variantType = getVariantType(getTypeTypeRef(), t_);
      void* variant = getVariantAddress(getTypeTypeRef(), t_);
      destroy(variantType, variant);
    }
    static void TypePrintFn(void* t_) {
      Type* t = (Type*)t_;
      U8* name;
      switch(t->kind) {
      case KIND_ARR:
        name = t->arr.name.utf8Data->data;
        break;
      case KIND_POB:
        name = t->pob.name.utf8Data->data;
        break;
      case KIND_REF:
      case KIND_VAL:
      case KIND_VAR:
        name = t->refValVar.name.utf8Data->data;
        break;
      }
      printf("#<Type {name: %s}>", name);
    }
    static Type TypeType = {
      KIND_VAR, {
        TypePrintFn,
        TypeDefaultFn,
        TypeDtorFn,
        sizeof(Type),
        sizeof(Uword),
        {
          sizeof("Type") - 1,
          &TypeTypeName
        },
        &TypeFields
      }
    };
    static TypeRef* TypeTypeRef = (TypeRef*)&TypeType;
    static TypeRef* getTypeTypeRef() {
      return TypeTypeRef;
    }
    static Array<U8> ArrayOfTypeTypeName = {
      sizeof("Type*"),
      "Type*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfTypeType = {
      KIND_ARR, {
        {
          sizeof("Type*") - 1,
          &ArrayOfTypeTypeName
        },
        TypeTypeRef
      }
    };
    static TypeRef* ArrayOfTypeTypeRef = (TypeRef*)&ArrayOfTypeType;
    // End Type definition

    // TypeRef definition
    static Array<U8> TypeRefTypeName = {
      sizeof("TypeRef"), // Is this the preferred way to name reference types?
      "TypeRef"
    };
    static Array<U8> TypeRefTypeFieldName = {
      sizeof("type"),
      "type"
    };
    static Array<Field> TypeRefFields = {
      1, {
        offsetof(TypeRef, t),
        TypeTypeRef,
        {
          sizeof("type") - 1,
          &TypeRefTypeFieldName
        }
      }
    };
    static void TypeRefDefaultFn(void* tr_) {
      TypeRef* trtr = getTypeRefTypeRef();
      copy(trtr, &trtr, tr_);
    }
    static void TypeRefDtorFn(void* tr_) {
      TypeRef* tr = *((TypeRef**)tr_);
      destroy(TypeTypeRef, &tr->t);
    }
    static void TypeRefPrintFn(void* tr_) {
      printf("#<TypeRef {??}>");
    }
    static Type TypeRefType = {
      KIND_REF, {
        TypeRefPrintFn,
        TypeRefDefaultFn,
        TypeRefDtorFn,
        sizeof(TypeRef),
        sizeof(Uword),
        {
          sizeof("TypeRef") - 1,
          &TypeRefTypeName
        },
        &TypeRefFields
      }
    };
    static TypeRef* TypeRefTypeRef = (TypeRef*)&TypeRefType;
    static TypeRef* getTypeRefTypeRef() {
      return TypeRefTypeRef;
    }
    static Array<U8> ArrayOfTypeRefTypeName = {
      sizeof("TypeRef*"),
      "TypeRef*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfTypeRefType = {
      KIND_ARR, {
        {
          sizeof("TypeRef*") - 1,
          &ArrayOfTypeRefTypeName
        },
        TypeRefTypeRef
      }
    };
    static TypeRef* ArrayOfTypeRefTypeRef = (TypeRef*)&ArrayOfTypeRefType;
    static TypeRef* getArrayOfTypeRefTypeRef() {
      return ArrayOfTypeRefTypeRef;
    }
    // End TypeRef definition

    // Symbol definition
    struct Symbol {
      String name;
    };
    static Array<U8> SymbolTypeName = {
      sizeof("Symbol"),
      "Symbol"
    };
    static Array<U8> SymbolNameFieldName = {
      sizeof("name"),
      "name"
    };
    static Array<Field> SymbolFields = {
      1, {
        offsetof(Symbol, name),
        StringTypeRef,
        {
          SymbolNameFieldName.size - 1,
          &SymbolNameFieldName
        }
      }
    };
    static void SymbolDefaultFn(void* sym_) {
      Symbol* sym = (Symbol*)sym_;
      StringDefaultFn(&sym->name);
    }
    static void SymbolDtorFn(void* sym_) {
      Symbol* sym = (Symbol*)sym_;
      StringDtorFn(&sym->name);
    }
    static void SymbolPrintFn(void* sym_) {
      Symbol* sym = (Symbol*)sym_;
      printf("%s", sym->name.utf8Data->data);
    }
    static Type SymbolType = {
      KIND_VAL, {
        SymbolPrintFn,
        SymbolDefaultFn,
        SymbolDtorFn,
        sizeof(Symbol),
        sizeof(Uword),
        {
          SymbolTypeName.size - 1,
          &SymbolTypeName
        },
        &SymbolFields
      }
    };
    static TypeRef* SymbolTypeRef = (TypeRef*)&SymbolType;
    static Array<U8> ArrayOfSymbolTypeName = {
      sizeof("Symbol*"),
      "Symbol*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfSymbolType = {
      KIND_ARR, {
        {
          ArrayOfSymbolTypeName.size - 1,
          &ArrayOfSymbolTypeName
        },
        SymbolTypeRef
      }
    };
    static TypeRef* ArrayOfSymbolTypeRef = (TypeRef*)&ArrayOfSymbolType;
    // End Symbol definition

    // Hashable protocol definition
    static Array<U8> HashableProtocolName = {
      sizeof("Hashable"),
      "Hashable"
    };
    static Array<U8> HashFunctionName = {
      sizeof("hash"),
      "hash"
    };
    static Array<TypeRef*> HashFunctionParams = {
      1,
      SelfTypeRef
    };
    static Array<TypeRef*> HashFunctionReturns = {
      1,
      UwordTypeRef
    };
    static Array<Function> HashableProtocolFunctions = {
      1,
      HashFunctionName.size - 1,
      &HashFunctionName,
      &HashFunctionParams,
      &HashFunctionReturns
    };
    static struct {
      Uword kind;
      ProtocolType pt;
    } HashableProtocolType = {
      KIND_POB, {
        {
          HashableProtocolName.size - 1,
          &HashableProtocolName
        },
        &HashableProtocolFunctions
      }
    };
    static TypeRef* HashableProtocolTypeRef = (TypeRef*)&HashableProtocolType;
    // End Hashable protocol

    // EqComparable protocol definition
    static Array<U8> EqComparableProtocolName = {
      sizeof("EqComparable"),
      "EqComparable"
    };
    static Array<U8> EqualsFunctionName = {
      sizeof("equals"),
      "equals"
    };
    static Array<TypeRef*> EqualsFunctionParams = {
      2,
      SelfTypeRef,
      ObjectProtocolTypeRef
    };
    static Array<TypeRef*> EqualsFunctionReturns = {
      1,
      BoolTypeRef
    };
    static Array<Function> EqComparableProtocolFunctions = {
      1,
      EqualsFunctionName.size - 1,
      &EqualsFunctionName,
      &EqualsFunctionParams,
      &EqualsFunctionReturns
    };
    static struct {
      Uword kind;
      ProtocolType pt;
    } EqComparableProtocolType = {
      KIND_POB, {
        {
          EqComparableProtocolName.size - 1,
          &EqComparableProtocolName
        },
        &EqComparableProtocolFunctions
      }
    };
    static TypeRef* EqComparableProtocolTypeRef = (TypeRef*)&EqComparableProtocolType;
    // End EqComparable protocol

    // Sequence protocol
    static Array<U8> SequenceProtocolName = {
      sizeof("Sequence"),
      "Sequence"
    };
    static Array<U8> FirstFunctionName = {
      sizeof("first"),
      "first"
    };
    static Array<TypeRef*> FirstFunctionParams = {
      1,
      SelfTypeRef
    };
    static Array<TypeRef*> FirstFunctionReturns = {
      1,
      ObjectOptionTypeRef
    };
    static Array<U8> RestFunctionName = {
      sizeof("rest"),
      "rest"
    };
    static Array<TypeRef*> RestFunctionParams = {
      1,
      SelfTypeRef
    };
    static Array<TypeRef*> RestFunctionReturns = {
      1,
      ObjectOptionTypeRef
    };
    static Array<Function> SequenceProtocolFunctions = {
      2,
      FirstFunctionName.size - 1,
      &FirstFunctionName,
      &FirstFunctionParams,
      &FirstFunctionReturns,
      RestFunctionName.size - 1,
      &RestFunctionName,
      &RestFunctionParams,
      &RestFunctionReturns
    };
    static struct {
      Uword kind;
      ProtocolType pt;
    } SequenceProtocolType = {
      KIND_POB, {
        {
          SequenceProtocolName.size - 1,
          &SequenceProtocolName
        },
        &SequenceProtocolFunctions
      }
    };
    static TypeRef* SequenceProtocolTypeRef = (TypeRef*)&SequenceProtocolType;
    // End Sequence protocol

    // ListNodeOption definition
    struct ListNode;
    TypeRef* getListNodeTypeRef();
    static void ListNodeDtorFn(void* node_);
    enum ListNodeOptionVariants {
      LIST_NODE_OPTION_NOTHING = 0,
      LIST_NODE_OPTION_NODE
    };
    struct ListNodeOption {
      Uword variant;
      union {
        Nothing nothing;
        ListNode* node;
      };
    };
    static Array<U8> ListNodeOptionTypeName = {
      sizeof("ListNodeOption"),
      "ListNodeOption"
    };
    static Array<U8> ListNodeOptionNothingFieldName = {
      sizeof("nothing"),
      "nothing"
    };
    static Array<U8> ListNodeOptionNodeFieldName = {
      sizeof("node"),
      "node"
    };
    static TypeRef* tmpTypeRef = getListNodeTypeRef();
    static Array<Field> ListNodeOptionFields = {
      2, {
        offsetof(ListNodeOption, nothing),
        NothingTypeRef,
        {
          sizeof("nothing") - 1,
          &ListNodeOptionNothingFieldName
        },
        offsetof(ListNodeOption, node),
        tmpTypeRef,
        {
          sizeof("node") - 1,
          &ListNodeOptionNodeFieldName
        }
      }
    };
    static void ListNodeOptionDefaultFn(void* opt_) {
      ListNodeOption* self = (ListNodeOption*)opt_;
      self->variant = OBJECT_OPTION_NOTHING;
      self->nothing.dummyValue = 0;
    }
    static void ListNodeOptionDtorFn(void* opt_) {
      ListNodeOption* self = (ListNodeOption*)opt_;
      if(self->variant == LIST_NODE_OPTION_NODE) {
        ListNodeDtorFn(self->node);
      }
    }
    static void ListNodeOptionPrintFn(void* opt_) {
      ListNodeOption* self = (ListNodeOption*)opt_;
      printf("#<ListNodeOption {??}>");
    }
    static Type ListNodeOptionType = {
      KIND_VAR, {
        ListNodeOptionPrintFn,
        ListNodeOptionDefaultFn,
        ListNodeOptionDtorFn,
        sizeof(ListNodeOption),
        sizeof(Uword),
        {
          sizeof("ListNodeOption") - 1,
          &ListNodeOptionTypeName
        },
        &ListNodeOptionFields
      }
    };
    static TypeRef* ListNodeOptionTypeRef = (TypeRef*)&ListNodeOptionType;
    static Array<U8> ArrayOfListNodeOptionTypeName = {
      sizeof("ListNodeOption*"),
      "ListNodeOption*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfListNodeOptionType = {
      KIND_ARR, {
        {
          sizeof("ListNodeOption*") - 1,
          &ArrayOfListNodeOptionTypeName
        },
        ListNodeOptionTypeRef
      }
    };
    static TypeRef* ArrayOfListNodeOptionTypeRef = (TypeRef*)&ArrayOfListNodeOptionType;
    // End ListNodeOption definition

    // ListNode definition
    struct ListNode {
      ObjectOption value;
      ListNodeOption next;
    };
    static Array<U8> ListNodeTypeName = {
      sizeof("ListNode"),
      "ListNode"
    };
    static Array<U8> ListNodeValueFieldName = {
      sizeof("value"),
      "value"
    };
    static Array<U8> ListNodeNextFieldName = {
      sizeof("next"),
      "next"
    };
    static Array<Field> ListNodeFields = {
      2, {
        offsetof(ListNode, value),
        ObjectOptionTypeRef,
        {
          ListNodeValueFieldName.size - 1,
          &ListNodeValueFieldName
        },
        offsetof(ListNode, next),
        ListNodeOptionTypeRef,
        {
          ListNodeNextFieldName.size - 1,
          &ListNodeNextFieldName
        }
      }
    };
    static void ListNodeDefaultFn(void* node_) {
      ListNode* node = (ListNode*)node_;
      node->value.variant = OBJECT_OPTION_NOTHING;
      node->next.variant = LIST_NODE_OPTION_NOTHING;
    }
    static void ListNodeDtorFn(void* node_) {
      ListNode* node = (ListNode*)node_;
      ObjectOptionDtorFn(&node->value);
      ListNodeOptionDtorFn(&node->next);
    }
    static void ListNodePrintFn(void* node_) {
      ListNode* node = (ListNode*)node_;
      printf("ListNode\n");
    }
    static Type ListNodeType = {
      KIND_REF, {
        ListNodePrintFn,
        ListNodeDefaultFn,
        ListNodeDtorFn,
        sizeof(ListNode),
        sizeof(Uword),
        {
          ListNodeTypeName.size - 1,
          &ListNodeTypeName
        },
        &ListNodeFields
      }
    };
    static TypeRef* ListNodeTypeRef = (TypeRef*)&ListNodeType;
    static TypeRef* getListNodeTypeRef() {
      return ListNodeTypeRef;
    }
    static Array<U8> ArrayOfListNodeTypeName = {
      sizeof("ListNode*"),
      "ListNode*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfListNodeType = {
      KIND_ARR, {
        {
          ArrayOfListNodeTypeName.size - 1,
          &ArrayOfListNodeTypeName
        },
        ListNodeTypeRef
      }
    };
    static TypeRef* ArrayOfListNodeTypeRef = (TypeRef*)&ArrayOfListNodeType;
    // End ListNode definition

    // List definition
    struct List {
      ListNodeOption head;
    };
    static Array<U8> ListTypeName = {
      sizeof("List"),
      "List"
    };
    static Array<U8> ListHeadFieldName = {
      sizeof("head"),
      "head"
    };
    static Array<Field> ListFields = {
      1, {
        offsetof(List, head),
        ListNodeOptionTypeRef,
        {
          ListHeadFieldName.size - 1,
          &ListHeadFieldName
        }
      }
    };
    static void ListDefaultFn(void* lst_) {
      List* lst = (List*)lst_;
      ListNodeOptionDefaultFn(&lst->head);
    }
    static void ListDtorFn(void* lst_) {
      List* lst = (List*)lst_;
      ListNodeOptionDtorFn(&lst->head);
    }
    static void ListPrintFn(void* sym_) {
      printf("List");
    }
    static Type ListType = {
      KIND_VAL, {
        ListPrintFn,
        ListDefaultFn,
        ListDtorFn,
        sizeof(List),
        sizeof(Uword),
        {
          ListTypeName.size - 1,
          &ListTypeName
        },
        &ListFields
      }
    };
    static TypeRef* ListTypeRef = (TypeRef*)&ListType;
    static Array<U8> ArrayOfListTypeName = {
      sizeof("List*"),
      "List*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfListType = {
      KIND_ARR, {
        {
          ArrayOfListTypeName.size - 1,
          &ArrayOfListTypeName
        },
        ListTypeRef
      }
    };
    static TypeRef* ArrayOfListTypeRef = (TypeRef*)&ArrayOfListType;
    // End List definition

    // HashtableKey protocol definition
    struct HashtableKeyPFns {
      Uword(*hash)(void* obj);
      Bool(*equals)(void* obj1, void* obj2);
    };
    typedef PObject<Self, HashtableKeyPFns> HashtableKey;
    static Array<U8> HashtableKeyProtocolName = {
      sizeof("HashtableKey"),
      "HashtableKey"
    };
    static Array<Function> HashtableKeyProtocolFunctions = {
      2,
      HashFunctionName.size - 1,
      &HashFunctionName,
      &HashFunctionParams,
      &HashFunctionReturns,
      EqualsFunctionName.size - 1,
      &EqualsFunctionName,
      &EqualsFunctionParams,
      &EqualsFunctionReturns
    };
    static struct {
      Uword kind;
      ProtocolType pt;
    } HashtableKeyProtocolType = {
      KIND_POB, {
        {
          HashtableKeyProtocolName.size - 1,
          &HashtableKeyProtocolName
        },
        &HashtableKeyProtocolFunctions
      }
    };
    static TypeRef* HashtableKeyProtocolTypeRef = (TypeRef*)&HashtableKeyProtocolType;
    // End HashtableKey protocol

    // HashtableKeyOption definition
    enum HashtableKeyOptionVariants {
      HASHTABLE_KEY_OPTION_NOTHING = 0,
      HASHTABLE_KEY_OPTION_KEY = 1
    };
    struct HashtableKeyOption {
      Uword variant;
      union {
        Nothing nothing;
        HashtableKey key;
      };
    };
    static Array<U8> HashtableKeyOptionTypeName = {
      sizeof("HashtableKeyOption"),
      "HashtableKeyOption"
    };
    static Array<U8> HashtableKeyOptionNothingFieldName = {
      sizeof("nothing"),
      "nothing"
    };
    static Array<U8> HashtableKeyOptionKeyFieldName = {
      sizeof("key"),
      "key"
    };
    static Array<Field> HashtableKeyOptionFields = {
      2, {
        offsetof(HashtableKeyOption, nothing),
        NothingTypeRef,
        {
          HashtableKeyOptionNothingFieldName.size - 1,
          &HashtableKeyOptionNothingFieldName
        },
        offsetof(HashtableKeyOption, key),
        HashtableKeyProtocolTypeRef,
        {
          HashtableKeyOptionKeyFieldName.size - 1,
          &HashtableKeyOptionKeyFieldName
        }
      }
    };
    static void HashtableKeyOptionDefaultFn(void* opt_) {
      HashtableKeyOption* self = (HashtableKeyOption*)opt_;
      self->variant = HASHTABLE_KEY_OPTION_NOTHING;
      self->nothing.dummyValue = 0;
    }
    static void HashtableKeyOptionDtorFn(void* opt_) {
      HashtableKeyOption* self = (HashtableKeyOption*)opt_;
      if(self->variant == HASHTABLE_KEY_OPTION_KEY) {
        if(self->key.vtable->type->t.kind == KIND_VAL) {
          assert("Error: Value type used as key in HashtableKey protocol" && False);
          //destroy(self->key.vtable->type, self->object.ref);
        } else { // KIND_REF, double indirection expected by destroy
          destroy(self->key.vtable->type, &self->key.ref);
        }
      }
    }
    static void HashtableKeyOptionPrintFn(void* opt_) {
      HashtableKeyOption* self = (HashtableKeyOption*)opt_;
      printf("#<HashtableKeyOption {??}>");
    }
    static Type HashtableKeyOptionType = {
      KIND_VAR, {
        HashtableKeyOptionPrintFn,
        HashtableKeyOptionDefaultFn,
        HashtableKeyOptionDtorFn,
        sizeof(HashtableKeyOption),
        sizeof(Uword),
        {
          HashtableKeyOptionTypeName.size - 1,
          &HashtableKeyOptionTypeName
        },
        &HashtableKeyOptionFields
      }
    };
    static TypeRef* HashtableKeyOptionTypeRef = (TypeRef*)&HashtableKeyOptionType;
    static Array<U8> ArrayOfHashtableKeyOptionTypeName = {
      sizeof("HashtableKeyOption*"),
      "HashtableKeyOption*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfHashtableKeyOptionType = {
      KIND_ARR, {
        {
          ArrayOfHashtableKeyOptionTypeName.size - 1,
          &ArrayOfHashtableKeyOptionTypeName
        },
        HashtableKeyOptionTypeRef
      }
    };
    static TypeRef* ArrayOfHashtableKeyOptionTypeRef = (TypeRef*)&ArrayOfHashtableKeyOptionType;
    // End HashtableKeyOption definition

    // HashtableEntry definition
    struct HashtableEntry {
      HashtableKeyOption key;
      ObjectOption value;
    };
    static Array<U8> HashtableEntryTypeName = {
      sizeof("HashtableEntry"),
      "HashtableEntry"
    };
    static Array<U8> HashtableEntryKeyFieldName = {
      sizeof("key"),
      "key"
    };
    static Array<U8> HashtableEntryValueFieldName = {
      sizeof("value"),
      "value"
    };
    static Array<Field> HashtableEntryFields = {
      2, {
        offsetof(HashtableEntry, key),
        HashtableKeyOptionTypeRef,
        {
          HashtableEntryKeyFieldName.size - 1,
          &HashtableEntryKeyFieldName
        },
        offsetof(HashtableEntry, value),
        ObjectOptionTypeRef,
        {
          HashtableEntryValueFieldName.size - 1,
          &HashtableEntryValueFieldName
        }
      }
    };
    static void HashtableEntryDefaultFn(void* hte_) {
      HashtableEntry* hte = (HashtableEntry*)hte_;
      HashtableKeyOptionDefaultFn(&hte->key);
      ObjectOptionDefaultFn(&hte->value);
    }
    static void HashtableEntryDtorFn(void* hte_) {
      HashtableEntry* hte = (HashtableEntry*)hte_;
      HashtableKeyOptionDtorFn(&hte->key);
      ObjectOptionDtorFn(&hte->value);
    }
    static void HashtableEntryPrintFn(void* hte_) {
      HashtableEntry* hte = (HashtableEntry*)hte_;
      printf("HashtableEntry");
    }
    static Type HashtableEntryType = {
      KIND_VAL, {
        HashtableEntryPrintFn,
        HashtableEntryDefaultFn,
        HashtableEntryDtorFn,
        sizeof(HashtableEntry),
        sizeof(Uword),
        {
          HashtableEntryTypeName.size - 1,
          &HashtableEntryTypeName
        },
        &HashtableEntryFields
      }
    };
    static TypeRef* HashtableEntryTypeRef = (TypeRef*)&HashtableEntryType;
    static Array<U8> ArrayOfHashtableEntryTypeName = {
      sizeof("HashtableEntry*"),
      "HashtableEntry*"
    };
    static struct {
      Uword kind;
      ArrType arr;
    } ArrayOfHashtableEntryType = {
      KIND_ARR, {
        {
          ArrayOfHashtableEntryTypeName.size - 1,
          &ArrayOfHashtableEntryTypeName
        },
        HashtableEntryTypeRef
      }
    };
    static TypeRef* ArrayOfHashtableEntryTypeRef = (TypeRef*)&ArrayOfHashtableEntryType;
    // End HashtableEntry definition

    // Hashtable protocol definition
    struct HashtableProtocolPFns {
      ObjectOption(*get)(HashtableKey key);
      void(*put)(HashtableKey key, ObjectOption value);
    };
    typedef PObject<Self, HashtableProtocolPFns> Hashtable;
    static Array<U8> HashtableProtocolName = {
      sizeof("Hashtable"),
      "Hashtable"
    };
    static Array<U8> HashtableGetFunctionName = {
      sizeof("get"),
      "get"
    };
    static Array<U8> HashtablePutFunctionName = {
      sizeof("put"),
      "put"
    };
    static Array<TypeRef*> HashtableGetFunctionParams = {
      1,
      HashtableKeyProtocolTypeRef
    };
    static Array<TypeRef*> HashtableGetFunctionReturns = {
      1,
      ObjectOptionTypeRef
    };
    static Array<TypeRef*> HashtablePutFunctionParams = {
      2,
      HashtableKeyProtocolTypeRef,
      ObjectOptionTypeRef
    };
    static Array<TypeRef*> HashtablePutFunctionReturns = {
      0
    };
    static Array<Function> HashtableProtocolFunctions = {
      2,
      // get
      HashtableGetFunctionName.size - 1,
      &HashtableGetFunctionName,
      &HashtableGetFunctionParams,
      &HashtableGetFunctionReturns,
      // put
      HashtablePutFunctionName.size - 1,
      &HashtablePutFunctionName,
      &HashtablePutFunctionParams,
      &HashtablePutFunctionReturns
    };
    static struct {
      Uword kind;
      ProtocolType pt;
    } HashtableProtocolType = {
      KIND_POB, {
        {
          HashtableProtocolName.size - 1,
          &HashtableProtocolName
        },
        &HashtableProtocolFunctions
      }
    };
    static TypeRef* HashtableProtocolTypeRef = (TypeRef*)&HashtableProtocolType;
    // End Hashtable protocol definition

    // Hashtable definition

    // End Hashtable definition

    static TypeRef* createRefValVarType(String name, Uword kind, PrintFn printFn, DefaultFn defaultFn, DestructorFn dtorFn, Uword alignment, Array<Field>* fields) {
      TypeRef* result = (TypeRef*)alignedObjectMalloc(TypeRefTypeRef);
      result->t.kind = kind;
      result->t.refValVar.alignment = alignment;
      result->t.refValVar.defaultFn = defaultFn;
      result->t.refValVar.dtorFn = dtorFn;
      result->t.refValVar.fields = fields;
      result->t.refValVar.name = name;
      result->t.refValVar.size = 0;
      result->t.refValVar.printFn = printFn;
      if(fields->size > 0) {
        // calculate size and set offsets for REF or VAL types
        if(kind != KIND_VAR) {
          Uword alignSizeOn = 1;
          Uword offset = 0;
          for(Uword i = 0; i < fields->size; ++i) {
            offset = align(offset, getFieldAlignment(fields->data[i].type));
            fields->data[i].offset = offset;
            Uword fieldSize = getFieldSize(fields->data[i].type);
            alignSizeOn = clamp(max(alignSizeOn, fieldSize), 1, sizeof(Uword));
            offset += fieldSize;
          }
          result->t.refValVar.size = align(offset, alignSizeOn);
        }
        // calculate size and offsets for VAR type
        else {
          result->t.refValVar.size = sizeof(Uword);
          for(Uword i = 0; i < fields->size; ++i) {
            fields->data[i].offset = align(sizeof(Uword), getFieldAlignment(fields->data[i].type));
            Uword fieldSize = getFieldSize(fields->data[i].type);
            result->t.refValVar.size = max(result->t.refValVar.size, sizeof(Uword) + fields->data[i].offset + fieldSize);
          }
          result->t.refValVar.size = align(result->t.refValVar.size, sizeof(Uword));
        }
      }
      return result;
    }

    static TypeRef* createRefType(String name, PrintFn printFn, DefaultFn defaultFn, DestructorFn dtorFn, Uword alignment, Array<Field>* fields) {
      return createRefValVarType(name, KIND_REF, printFn, defaultFn, dtorFn, alignment, fields);
    }

    static TypeRef* createValType(String name, PrintFn printFn, DefaultFn defaultFn, DestructorFn dtorFn, Uword alignment, Array<Field>* fields) {
      return createRefValVarType(name, KIND_VAL, printFn, defaultFn, dtorFn, alignment, fields);
    }

    static TypeRef* createVarType(String name, PrintFn printFn, DefaultFn defaultFn, DestructorFn dtorFn, Uword alignment, Array<Field>* fields) {
      return createRefValVarType(name, KIND_VAR, printFn, defaultFn, dtorFn, alignment, fields);
    }



    // ******************************** TESTS ********************************

    static void testCopyMoveUword() {
      Uword src = 10;
      Uword dest;
      defaultInitObject(UwordTypeRef, &dest);
      assert(dest == 0);
      copy(UwordTypeRef, &src, &dest);
      assert(dest == 10);
      defaultInitObject(UwordTypeRef, &dest);
      assert(dest == 0);
      assert(src == 10);
      move(UwordTypeRef, &src, &dest);
      assert(dest == 10);
      assert(src == 0);
    }

    static void testCopyMoveString() {
      String s1 = stringFromCString("Hello");
      String s2 = stringFromCString("World");
      String s3;
      defaultInitObject(StringTypeRef, &s3);

      String s4;
      move(StringTypeRef, &s1, &s4);
      String s5;
      copy(StringTypeRef, &s2, &s5);

      destroy(StringTypeRef, &s5);
      destroy(StringTypeRef, &s4);
      destroy(StringTypeRef, &s3);
      destroy(StringTypeRef, &s2);
      destroy(StringTypeRef, &s1);
    }

    static void testCopyMoveArrayType() {
      ArrType atNothing;
      defaultInitObject(ArrayTypeTypeRef, &atNothing);
      ArrType nothingCopy;
      copy(ArrayTypeTypeRef, &atNothing, &nothingCopy);
      ArrType movedTo;
      move(ArrayTypeTypeRef, &atNothing, &movedTo);
      destroy(ArrayTypeTypeRef, &nothingCopy);
      destroy(ArrayTypeTypeRef, &movedTo);
    }

    static void testCopyMoveField() {
      Field f1;
      defaultInitObject(FieldTypeRef, &f1);
      Field f2;
      copy(FieldTypeRef, &f1, &f2);
      Field f3;
      move(FieldTypeRef, &f2, &f3);
      destroy(FieldTypeRef, &f1);
      destroy(FieldTypeRef, &f3);
    }

    struct TEST_StringRef {
      String str;
    };

    static void TEST_StringRefDefaultFn(void* sr_) {
      TEST_StringRef* sr = *((TEST_StringRef**)sr_);
      defaultInitObject(StringTypeRef, &sr->str);
    }

    static void TEST_StringRefPrintFn(void* sr_) {
      TEST_StringRef* sr = *((TEST_StringRef**)sr_);
      StringTypeRef->t.refValVar.printFn(&sr->str);
    }

    static void TEST_StringRefDtorFn(void* sr_) {
      TEST_StringRef* sr = *((TEST_StringRef**)sr_);
      destroy(StringTypeRef, &sr->str);
    }

    static void createAndCopyMoveStringRefType() {
      Array<Field>* fields = (Array<Field>*)alignedArrayMalloc(FieldTypeRef, 1);
      fields->size = 1;
      fields->data[0].name = stringFromCString("str");
      fields->data[0].offset = 0;
      copy(TypeRefTypeRef, &StringTypeRef, &fields->data[0].type);
      TypeRef* theType = createRefType(stringFromCString("StringRef"), TEST_StringRefPrintFn, TEST_StringRefDefaultFn, TEST_StringRefDtorFn, sizeof(Uword), fields);

      TEST_StringRef* instance = (TEST_StringRef*)alignedObjectMalloc(theType);
      TEST_StringRef* cp;
      TEST_StringRef* mv;

      TEST_StringRef compare;
      Array<U8> hej = { 0 };
      compare.str.utf8Data = &hej;
      compare.str.size = 0;

      defaultInitObject(theType, &instance);

      assert(compare.str.size == instance->str.size);
      assert(compare.str.utf8Data->size == instance->str.utf8Data->size);
      assert(memcmp(compare.str.utf8Data->data, instance->str.utf8Data->data, instance->str.utf8Data->size) == 0);

      copy(theType, &instance, &cp);

      assert(compare.str.size == cp->str.size);
      assert(compare.str.utf8Data->size == cp->str.utf8Data->size);
      assert(memcmp(compare.str.utf8Data->data, cp->str.utf8Data->data, compare.str.utf8Data->size) == 0);

      move(theType, &instance, &mv);

      assert(compare.str.size == mv->str.size);
      assert(compare.str.utf8Data->size == mv->str.utf8Data->size);
      assert(memcmp(compare.str.utf8Data->data, mv->str.utf8Data->data, compare.str.utf8Data->size) == 0);

      destroy(theType, &cp);
      destroy(theType, &mv);
      destroy(TypeRefTypeRef, &theType);
    }

    int main(int argc, char* argv[]) {
      testCopyMoveUword();
      testCopyMoveString();
      testCopyMoveArrayType();
      testCopyMoveField();
      createAndCopyMoveStringRefType();
    }
