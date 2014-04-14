#ifndef OCT_SYSTEM_H
#define OCT_SYSTEM_H

#include "Primitives.h"

namespace octarine {

  struct System { // container for platform functions that the runtime depends on
	static Address malloc(Uword size);
  };

} // ns octarine

#endif

