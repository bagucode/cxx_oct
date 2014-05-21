#include "Context.h"

namespace octarine {

  Runtime* getRuntime(Context* ctx) {
	return ctx->runtime;
  }

}
