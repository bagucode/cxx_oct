#ifndef OCT_PROTOCOL_H
#define OCT_PROTOCOL_H

#include "Primitives.h"
#include "VTable.h"

namespace octarine {

  template <typename FNs>
  struct ProtocolObject {
	Address object;
	VTable<FNs>* vtable;
  };

} // ns octarine

#endif
