#include "Array.h"

namespace octarine {

  struct Array {
	Type* elementType;
	Uword size;
	Address data;
  };

  Array* createArray(Context* ctx, Type* elementType, Uword size) {
	// TBD
  }

  Object asObject(Context* ctx, Array* arr) {
	// TBD
  }

  Array* asArray(Context* ctx, Object obj) {
	// TBD
  }

  Object getValue(Context* ctx, Array* arr, Uword index) {
  }

  void setValue(Context* ctx, Array* arr, Uword index, Object value) {
  }

  Uword getSize(Context* ctx, Array* arr) {
  }

  Type* getElementType(Context* ctx, Array* arr) {
  }

} // ns octarine

