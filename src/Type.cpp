#include "Type.h"
#include "Field.h"
#include <stddef.h>

namespace octarine {

	Field _TypeFields [] = {
		{ UwordType, VALUE, offsetof(Type, _size), { "size" } },
		{ UwordType, VALUE, offsetof(Type, _alignment), { "alignment" } },
		{ ArrayType, VALUE, offsetof(Type, _fields), { "fields" } }
	};

	Array _TypeFieldsArray = {
		Field::type,
		3,
		&_TypeFields
	};

	Type _TypeType = {
		sizeof(Type),
		sizeof(Uword),
		_TypeFieldsArray
	};

	Type* Type::type = &_TypeType;

	static void _init(Self* self) {
		Type* t = (Type*) self;
		t->_alignment = 0;
		t->_fields.init();
		t->_size = 0;
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return Type::type;
	}

	static Uword _hash(Self* self) {
		Type* t = (Type*) self;
		Uword hash = 17;
		hash += t->_size * 37;
		hash += t->_alignment * 37;
		return hash + t->_fields.hash() * 37;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.self) {
			return True;
		}
		if (other.functions->type(other.self) != Type::type) {
			return False;
		}
		Type* t = (Type*) self;
		Type* otherT = (Type*) other.self;
		return t->_size == otherT->_size &&
			t->_alignment == otherT->_alignment &&
			t->_fields.equals(&otherT->_fields);
	}

	static void _trace(Self* self, MemoryManager mm) {
		Type* t = (Type*) self;
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
