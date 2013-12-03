#include "Type.h"
#include "Field.h"
#include <stddef.h>

namespace octarine {

	Field _TypeFields [] = {
		{ UwordType, VALUE, offsetof(Type, size), "size" },
		{ UwordType, VALUE, offsetof(Type, alignment), "alignment" },
		{ ArrayType, VALUE, offsetof(Type, fields), "fields" }
	};

	Array _TypeFieldsArray = {
		FieldType,
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
		t->alignment = 0;
		t->fields.init();
		t->size = 0;
	}

	static void _destroy(Self* self) {
		Type* t = (Type*) self;
		t->fields.destroy();
	}

	static Type* _type(Self* self) {
		return Type::type;
	}

	static Uword _hash(Self* self) {
		Type* t = (Type*) self;
		Uword hash = 17;
		hash += t->size * 37;
		hash += t->alignment * 37;
		return hash + t->fields.hash() * 37;
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
		return t->size == otherT->size &&
			t->alignment == otherT->alignment &&
			t->fields.equals(&otherT->fields);
	}

	static void _trace(Self* self, MemoryManager mm) {
		Type* t = (Type*) self;
		Uword markResult;
		mm.functions->mark(mm.self, t, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		t->fields.trace(mm);
	}

	ObjectFunctions Type::objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	Object Type::asObject() {
		return{ (Self*)this, &Type::objectFns };
	}

}
