#include "Array.h"
#include "Field.h"
#include "Type.h"
#include "Nothing.h"
#include <stddef.h>

namespace octarine {

	struct Array {
		Type* _type;
		Uword _size;
		Address _data;

		static Type* type;
		static ObjectFunctions objectFns;
		Object asObject();
	};


	Field _ArrayFields[] = {
		{ Type::type, POINTER, offsetof(Array, _type), { "type" } },
		{ UwordType, VALUE, offsetof(Array, _size), { "size" } },
		{ AddressType, VALUE, offsetof(Array, _data), { "data" } }
	};

	Array _ArrayFieldsArray = {
		Field::type,
		3,
		&_TypeFields
	};

	Type _ArrayType = {
		sizeof(Array),
		sizeof(Uword),
		_ArrayFieldsArray
	};

	Type* Array::type = &_ArrayType;

	static void _init(Self* self) {
		Array* a = (Array*) self;
		a->_data = nullptr;
		a->_size = 0;
		a->_type = Nothing::type;
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return Array::type;
	}

	static Uword _hash(Self* self) {
		Array* a = (Array*) self;
		Uword hash = 17;
		hash += a->_size * 37;
		hash += a->_type->objectFns.hash((Self*)a->_type) * 37;
		for (Uword i = 0; i < a->_size; ++i) {
			hash += 
		}
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.self) {
			return True;
		}
		if (other.functions->type(other.self) != Type::type) {
			return False;
		}
		Array* a = (Array*) self;
		Type* otherT = (Type*) other.self;
		return t->_size == otherT->_size &&
			t->_alignment == otherT->_alignment &&
			t->_fields.equals(&otherT->_fields);
	}

	static void _trace(Self* self, MemoryManager mm) {
		Array* a = (Array*) self;
		Uword markResult;
		mm.functions->mark(mm.self, t, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		t->_fields.trace(mm);
	}

	ObjectFunctions Type::objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	Object Type::asObject() {
		return{ (Self*)this, &Type::objectFns };
	}


}
