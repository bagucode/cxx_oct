#include "Field.h"
#include "Type.h"
#include <stddef.h>

namespace octarine {

	Field _FieldFields[] = {
		{ Type::type, POINTER, offsetof(Field, _type), { "type" } },
		{ UwordType, VALUE, offsetof(Field, _kind), { "kind" } },
		{ UwordType, VALUE, offsetof(Field, _offset), { "offset" } },
		{ String::type, VALUE, offsetof(Field, _name), { "name" } }
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
	}

	static Type* _typeFn(Self* self) {
		return Field::type;
	}

	static Uword _hash(Self* self) {
		Field* f = (Field*) self;
		Uword hash = 17;
		hash += f->_kind * 37;
		hash += f->_name.objectFns.hash((Self*)&f->_name) * 37;
		hash += f->_offset * 37;
		hash += f->_type->objectFns.hash((Self*)f->_type) * 37;
		return hash;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.self) {
			return True;
		}
		if (other.functions->type(other.self) != Field::type) {
			return False;
		}
		Field* f = (Field*) self;
		Field* otherF = (Field*) other.self;
		return f->_kind == otherF->_kind &&
			f->_name.objectFns.equals((Self*)&f->_name, otherF->_name.asObject()) &&
			f->_offset == otherF->_offset &&
			f->_type->objectFns.equals((Self*)f->_type, otherF->_type->asObject());
	}

	static void _trace(Self* self, MemoryManager mm) {
		Field* f = (Field*) self;
		Uword markResult;
		mm.functions->mark(mm.self, f, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		f->_name.objectFns.trace((Self*)&f->_name, mm);
		f->_type->objectFns.trace((Self*)f->_type, mm);
	}

	ObjectFunctions Field::objectFns = { _init, _destroy, _typeFn, _hash, _equals, _trace };

	Object Field::asObject() {
		return{ (Self*)this, &Type::objectFns };
	}

}
