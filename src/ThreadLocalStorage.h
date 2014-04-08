#ifndef OCT_THREADLOCALSTORAGE_H
#define OCT_THREADLOCALSTORAGE_H

#include "Primitives.h"

#ifdef WINDOWS
#include <Windows.h>
#elif defined (MACOSX)
#include <pthread.h>
#else
#include <pthread.h>
#endif

namespace octarine {

  class TLS {
  private:
#ifdef WINDOWS
	DWORD threadLocalStorage;
#elif defined (MACOSX)
	pthread_key_t threadLocalStorage;
#else
	pthread_key_t threadLocalStorage;
#endif
  public:
	TLS() {
#ifdef WINDOWS
	  threadLocalStorage = TlsAlloc();
#elif defined (MACOSX)
	  pthread_key_create(&threadLocalStorage, NULL);
#else
	  pthread_key_create(&threadLocalStorage, NULL);
#endif
	}

	~TLS() {
#ifdef WINDOWS
	  TlsFree(threadLocalStorage);
#elif defined (MACOSX)
	  pthread_key_delete(threadLocalStorage);
#else
	  pthread_key_delete(threadLocalStorage);
#endif
	}

	void set(void* value) {
#ifdef WINDOWS
	  TlsSetValue(threadLocalStorage, value);
#elif defined (MACOSX)
	  pthread_setspecific(threadLocalStorage, value);
#else
	  pthread_setspecific(threadLocalStorage, value);
#endif
	}

	void* get() {
#ifdef WINDOWS
	  return TlsGetValue(threadLocalStorage);
#elif defined (MACOSX)
	  return pthread_getspecific(threadLocalStorage);
#else
	  return pthread_getspecific(threadLocalStorage);
#endif
	}
  };

} // namespace


#endif
