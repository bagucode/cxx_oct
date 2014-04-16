#ifndef OCT_VTABLE_H
#define OCT_VTABLE_H

namespace octarine {

  struct Type;

  template <typename FNs>
  struct VTable {
  private:
	Type* _type;
	FNs* _functions;

  public:
	Type* getType() {
	  return _type;
	}

	FNs* getFunctions() {
	  return _functions;
	}
  };

};

#endif

