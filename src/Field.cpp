#include "Field.h"
#include "Type.h"
#include <stddef.h>

namespace octarine {

	Field _FieldFields[] = {
		{ Type::type, POINTER, offsetof(Field, _type), "type" },
		{ UwordType, VALUE, offsetof(Field, _kind), "kind" },
		{ UwordType, VALUE, offsetof(Field, _offset), "offset" },
		{ String::type, VALUE, offsetof(Field, _name), "name"}
	};

	Array _FieldFieldsArray = {
		Field::type,
		4,
		&_FieldFields
	};

	Type _FieldType = {
		sizeof(Field),
		sizeof(Uword),
		_FieldFieldsArray
	};

	Type* Field::type = &_FieldType;

	static void _init(Self* self) {
		Field* f = (Field*) self;
		f->_type->objectFns.init((Self*) f->type);
		f->_kind = VALUE;
		f->_offset = 0;
		f->_name.objectFns.init((Self*) &f->_name);
	}

	static void _destroy(Self* self) {
		Field* f = (Field*) self;
		t->fields.destroy();
	}

	static Type* _type(Self* self) {
		return Type::type;
	}

	static Uword _hash(Self* self) {
		Field* f = (Field*) self;
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
		Field* f = (Field*) self;
		Type* otherT = (Type*) other.self;
		return t->size == otherT->size &&
			t->alignment == otherT->alignment &&
			t->fields.equals(&otherT->fields);
	}

	static void _trace(Self* self, MemoryManager mm) {
		Field* f = (Field*) self;
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
