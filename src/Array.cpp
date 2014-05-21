#include "Array.h"

namespace octarine {

  struct Array {
	Type* elementType;
	Uword size;
	Address data;
  };

  void _initArrayType(Context* ctx) {
	// TBD
  }

  Array* createArray(Context* ctx, Type* elementType, Uword size) {
	// TBD
  }

  Object asObject(Context* ctx, Array* arr) {
	// TBD
  }

  Array* asArray(Context* ctx, Object obj) {
	// TBD
  }

} // ns octarine

