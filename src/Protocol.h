#ifndef OCT_PROTOCOL_H
#define OCT_PROTOCOL_H

#include "Type.h"

namespace octarine {

  struct ProtocolData {
	Type _type;
	Address _value;
  };

  template <typename T>
  struct ProtocolFunctions {
	Type getType() {
	  return _protocolData._type;
	}
	Address getValue() {
	  return _protocolData._value;
	}
  };

} // ns octarine

#endif
