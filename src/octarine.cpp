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
#include <pthread.h>
#endif
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>
#include <cstring>

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

	struct SharedMemoryManagerT;
	typedef SharedMemoryManagerT* SharedMemoryManager;

	struct ThreadMemoryManagerT;
	typedef ThreadMemoryManagerT* ThreadMemoryManager;

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

	static Uword getSize(ThreadContext tc, Type t);

	static SharedMemoryManager createSharedMemoryManager();

	static void destroySharedMemoryManager(SharedMemoryManager mm);

	static ThreadMemoryManager createThreadMemoryManager();

	static void destroyThreadMemoryManager(ThreadMemoryManager mm);

	static Object allocRaw(ThreadMemoryManager mm, Uword size);

	static Object alloc(ThreadContext tc, ThreadMemoryManager mm, Type t);

	static Object allocRaw(SharedMemoryManager mm, Uword size);

	static Object alloc(ThreadContext tc, SharedMemoryManager mm, Type t);

	static Namespace createNamespace(ThreadContext tc, String name);

	static Object bind(ThreadContext tc, Namespace ns, String name, Object obj);

	static Type getNamespaceType(Runtime rt);

	static Bool equals(ThreadContext tc, Namespace ns, Object obj);

	static Type getType(ThreadContext tc, Object obj);

	static Uword hash(ThreadContext tc, Object o);

	static Bool equals(ThreadContext tc, Object o1, Object o2);

	static String makeShared(ThreadContext tc, String s);

	static Namespace makeShared(ThreadContext tc, Namespace s);

	static Runtime createRuntime();

	static void destroyRuntime(Runtime rt);

	static void registerFunction(ThreadContext tc, Namespace ns, Function f);

	static Object eval(ThreadContext tc, String source);

	static Object intern(ThreadContext tc, Object o);

	static String createString(ThreadContext tc, const char* cstr);

	static Uword hash(ThreadContext tc, String s);

	static Bool equals(ThreadContext tc, String s1, String s2);

	static ThreadContext createThreadContext(Runtime rt, ThreadMemoryManager mm, Namespace initialNs);

	static void destroyThreadContext(ThreadContext tc);

	static ThreadContext getThreadContext();

	static Runtime getRuntime(ThreadContext tc);

	static SharedMemoryManager getSharedMemoryManager(Runtime rt);

	static ThreadMemoryManager getThreadMemoryManager(ThreadContext tc);

	static Namespace getNamespace(ThreadContext tc);

	static Namespace setNamespace(ThreadContext tc, Namespace ns);

	static Exception createException(ThreadContext tc, String message);

	static String getMessage(ThreadContext tc, Exception e);

	// ## 40 Type definitions

	// STL Integration
}
namespace std {
	template <>
	struct hash<octarine::String> {
		size_t operator()(const octarine::String& s) const {
			return octarine::hash(octarine::getThreadContext(), s);
		}
	};

	template<>
	struct equal_to<octarine::String> {
		bool operator()(const octarine::String& s1, const octarine::String& s2) const {
			return octarine::equals(octarine::getThreadContext(), s1, s2) == True ? true : false;
		}
	};

	template<>
	struct hash<octarine::Object> {
		size_t operator()(const octarine::Object& o) const {
			return octarine::hash(octarine::getThreadContext(), o);
		}
	};

	template<>
	struct equal_to<octarine::Object> {
		bool operator()(const octarine::Object& o1, const octarine::Object& o2) {
			return octarine::equals(octarine::getThreadContext(), o1, o2) == True ? true : false;
		}
	};
}
namespace octarine {

	// Platform support

	class TLS {
	private:
#ifdef WINDOWS
		DWORD threadLocalStorage;
#elif defined (MACOSX)
		pthread_key_t threadLocalStorage;
#else
#endif
	public:
		TLS() {
#ifdef WINDOWS
			threadLocalStorage = TlsAlloc();
#elif defined (MACOSX)
			pthread_key_create(&threadLocalStorage, nullptr);
#else
#endif
		}

		~TLS() {
#ifdef WINDOWS
			TlsFree(threadLocalStorage);
#elif defined (MACOSX)
			pthread_key_delete(threadLocalStorage);
#else
#endif
		}

		void set(void* value) {
#ifdef WINDOWS
			TlsSetValue(threadLocalStorage, value);
#elif defined (MACOSX)
			pthread_setspecific(threadLocalStorage, value);
#else
#endif
		}

		void* get() {
#ifdef WINDOWS
			return TlsGetValue(threadLocalStorage);
#elif defined (MACOSX)
			return pthread_getspecific(threadLocalStorage);
#else
#endif
		}
	};

	// Octarine types

	struct TypeT {
		Uword size;
	};

	struct ObjectHeader {
		Type type;
		Uword object;
	};

	struct SharedMemoryManagerT {
		Uword hello;
	};

	struct ThreadMemoryManagerT {
		Uword hello;
	};

	struct NamespaceT {
		String name;
	};

	struct StringT {
		Uword length;
		U8* utf8data; // Just use platform strings?
	};

	struct BuiltInTypes {
		Type namespaceType;
		Type stringType;
	};

	struct ThreadContextT {
		Runtime runtime;
		ThreadMemoryManager ThreadMemoryManager;
		Namespace currentNs;
	};

	struct RuntimeT {
		SharedMemoryManager sharedMemoryManager;
		std::mutex namespaceMutex;
		std::unordered_map<String, Namespace> namespaces;
		std::mutex threadContextMutex;
		std::vector<ThreadContext> threadContexts;
		std::mutex internedObjectsMutex;
		std::unordered_set<Object> internedObjects;
		BuiltInTypes builtInTypes;
	};

    // ## 50 Function definitions

	// Global thread local var for getting at the current thread context when it is impossible
	// to pass it along. For example when integrating with the STL or in callbacks from C code.
	static TLS tls;

	static Uword getSize(ThreadContext tc, Type t) {
		return t->size;
	}

	static SharedMemoryManager createSharedMemoryManager() {
		SharedMemoryManager mm = new SharedMemoryManagerT;
		return mm;
	}

	static void destroySharedMemoryManager(SharedMemoryManager mm) {
		delete mm;
	}

	static ThreadMemoryManager createThreadMemoryManager() {
		ThreadMemoryManager mm = new ThreadMemoryManagerT;
		return mm;
	}

	static void destroyThreadMemoryManager(ThreadMemoryManager mm) {
		delete mm;
	}

	static Object allocRaw(SharedMemoryManager mm, Uword size) {
		Uword allocSize = sizeof(ObjectHeader) + size;
		ObjectHeader* oh = (ObjectHeader*)::operator new(allocSize);
		oh->type = nullptr;
		return (Object) &oh->object;
	}

	static Object alloc(ThreadContext tc, SharedMemoryManager mm, Type t) {
		Uword allocSize = sizeof(ObjectHeader) + getSize(tc, t);
		ObjectHeader* oh = (ObjectHeader*)::operator new(allocSize);
		oh->type = t;
		return (Object) &oh->object;
	}

	static Object allocRaw(ThreadMemoryManager mm, Uword size) {
		Uword allocSize = sizeof(ObjectHeader) + size;
		ObjectHeader* oh = (ObjectHeader*)::operator new(allocSize);
		oh->type = nullptr;
		return (Object) &oh->object;
	}

	static Object alloc(ThreadContext tc, ThreadMemoryManager mm, Type t) {
		Uword allocSize = sizeof(ObjectHeader) + getSize(tc, t);
		ObjectHeader* oh = (ObjectHeader*)::operator new(allocSize);
		oh->type = t;
		return (Object) &oh->object;
	}

	static Namespace createNamespace(ThreadContext tc, String name) {
		ThreadMemoryManager mm = getThreadMemoryManager(tc);
		Runtime rt = getRuntime(tc);
		Namespace ns = (Namespace) allocRaw(mm, sizeof(NamespaceT));
		ns->name = name;
		rt->namespaces[name] = ns;
		return makeShared(tc, ns);
	}

	static Object bind(ThreadContext tc, Namespace ns, String name, Object obj) {

	}

	static Type getStringType(Runtime rt) {
		return rt->builtInTypes.stringType;
	}

	static Type getNamespaceType(Runtime rt) {
		return rt->builtInTypes.namespaceType;
	}

	static Bool equals(ThreadContext tc, Namespace ns, Object obj) {

	}

	static Type getType(ThreadContext tc, Object obj) {

	}

	static Uword hash(ThreadContext tc, Object o) {

	}

	static Bool equals(ThreadContext tc, Object o1, Object o2) {

	}

	static String makeShared(ThreadContext tc, String s) {
		Runtime rt = getRuntime(tc);
		SharedMemoryManager mm = getSharedMemoryManager(rt);
		size_t len = strlen((const char*) s->utf8data); // To get the byte length. s->length is number of codepoints
		String sharedS = (String) alloc(tc, mm, getStringType(rt));
		sharedS->utf8data = (U8*) allocRaw(mm, len + 1);
		memcpy(sharedS->utf8data, s->utf8data, len + 1);
		sharedS->length = s->length;
		return sharedS;
	}

	static Namespace makeShared(ThreadContext tc, Namespace s) {
		Runtime rt = getRuntime(tc);
		Namespace sharedNs = (Namespace)alloc(tc, getSharedMemoryManager(rt), getNamespaceType(rt));
		sharedNs->name = makeShared(tc, s->name);
		return sharedNs;
	}

	static Runtime createRuntime() {
		Runtime rt = new RuntimeT;
		rt->sharedMemoryManager = createSharedMemoryManager();
		// Create main thread context, with its own memory manager
		ThreadContext mainTc = createThreadContext(rt, createThreadMemoryManager(), nullptr);
		rt->threadContexts.push_back(mainTc);
		// Create main namespace
		setNamespace(mainTc, createNamespace(mainTc, createString(mainTc, "octarine")));
		return rt;
	}

	static void destroyRuntime(Runtime rt) {
		std::for_each(rt->threadContexts.begin(), rt->threadContexts.end(), destroyThreadContext);
		delete rt;
	}

	static void registerFunction(ThreadContext tc, Namespace ns, Function f) {

	}

	static Object eval(ThreadContext tc, String source) {

	}

	static Object intern(ThreadContext tc, Object o) {
		Runtime rt = getRuntime(tc);
		std::lock_guard<std::mutex> lock(rt->internedObjectsMutex);
		std::unordered_set<Object>::const_iterator i = rt->internedObjects.find(o);
		if (i == rt->internedObjects.end()) {
			// Not found, copy to global memory and insert.
			return nullptr;
		}
		else {
			return *i;
		}
	}

	static String createString(ThreadContext tc, const char* cstr) {
		return nullptr;
	}

	static Uword hash(ThreadContext tc, String s) {
		return 0;
	}

	static Bool equals(ThreadContext tc, String s1, String s2) {
		return False;
	}

	static ThreadContext createThreadContext(Runtime rt, ThreadMemoryManager mm, Namespace initialNs) {
		ThreadContext tc = (ThreadContext) allocRaw(mm, sizeof(ThreadContextT));
		tc->runtime = rt;
		tc->ThreadMemoryManager = mm;
		tc->currentNs = initialNs;
		tls.set(tc);
		return tc;
	}

	static void destroyThreadContext(ThreadContext tc) {
		tls.set(nullptr);
		destroyThreadMemoryManager(tc->ThreadMemoryManager);
	}

	static ThreadContext getThreadContext() {
		return (ThreadContext) tls.get();
	}

	static Runtime getRuntime(ThreadContext tc) {
		return tc->runtime;
	}

	static SharedMemoryManager getSharedMemoryManager(Runtime rt) {
		return rt->sharedMemoryManager;
	}

	static ThreadMemoryManager getThreadMemoryManager(ThreadContext tc) {
		return tc->ThreadMemoryManager;
	}

	static Namespace getNamespace(ThreadContext tc) {
		return tc->currentNs;
	}

	static Namespace setNamespace(ThreadContext tc, Namespace ns) {
		Namespace old = tc->currentNs;
		tc->currentNs = ns;
		return old;
	}

	static Exception createException(ThreadContext tc, String message) {

	}

	static String getMessage(ThreadContext tc, Exception e) {

	}

    
}

// ## 60 C API

#endif
