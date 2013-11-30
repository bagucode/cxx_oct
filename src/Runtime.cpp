#include "Runtime.h"
#include "Primitives.h"

namespace octarine {

	class TLS {
	private:
		#ifdef WINDOWS
			DWORD threadLocalStorage;
		#elif defined (MACOSX)
		#else
		#endif
	public:
		TLS() {
			#ifdef WINDOWS
				threadLocalStorage = TlsAlloc();
			#elif defined (MACOSX)
			#else
			#endif
		}
		
		~TLS() {
			#ifdef WINDOWS
				TlsFree(threadLocalStorage);
			#elif defined (MACOSX)
			#else
			#endif
		}

		void set(void* value) {
			#ifdef WINDOWS
				TlsSetValue(threadLocalStorage, value);
			#elif defined (MACOSX)
			#else
			#endif
		}

		void* get() {
			#ifdef WINDOWS
				return TlsGetValue(threadLocalStorage);
			#elif defined (MACOSX)
                return nullptr;
			#else
			#endif
		}
	};

	struct RuntimeImpl {
		TLS tls;
	};

	Runtime::Runtime(): _impl(new RuntimeImpl) {
	}

	Runtime::~Runtime() {
		delete _impl;
	}

	static void _registerFunction(ThreadContext tc, Runtime* rt, Namespace ns, Function f) {

	}

	void Runtime::registerFunction(ThreadContext tc, Namespace ns, Function f) {
		_registerFunction(tc, this, ns, f);
	}

	ThreadContext Runtime::getThreadContext() {
        return nullptr;
	}

}

