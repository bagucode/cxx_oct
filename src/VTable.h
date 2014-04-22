#ifndef OCT_VTABLE_H
#define OCT_VTABLE_H

namespace octarine {

  struct Type;

  template <typename FNs>
  struct VTable {
	Type* type;
	FNs functions;
  };

};

#endif

