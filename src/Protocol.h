#ifndef OCT_PROTOCOL_H
#define OCT_PROTOCOL_H

#include "Primitives.h"
#include "VTable.h"

namespace octarine {

  template <typename FNs>
  struct Protocol {
  private:
	Address _object;
	VTable<FNs>* _vtable;
  public:
	Protocol(Address object, VTable<FNs>* vtable):
	  _object(object), _vtable(vtable) {
	}

	Address getValue() {
	  return _object;
	}

	VTable<FNs>* getVTable() {
	  return _vtable;
	}
  };

} // ns octarine

#endif
