#ifndef OCT_READER_H
#define OCT_READER_H

#include "Object.h"
#include "CharStream.h"

namespace octarine {

  struct Reader {
	Object ReadObject(Context* ctx, CharStream* stream);
  }

}

#endif

