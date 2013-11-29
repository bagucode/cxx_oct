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
			#else
			#endif
		}
	};

	struct RuntimeT {
		TLS tls;
	};

	void registerFunction(ThreadContext tc, Runtime rt, Namespace ns, Function f) {
	}

	ThreadContext getThreadContext(Runtime rt) {

	}

	Runtime createRuntime() {
		Runtime rt = new RuntimeT;

		return rt;
	}

	void destroyRuntime(Runtime rt) {
		delete rt;
	}

}

