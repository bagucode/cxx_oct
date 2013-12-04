#include "Field.h"
#include "Type.h"
#include "Nothing.h"
#include <stddef.h>

namespace octarine {

	Field _FieldFields[] = {
		{ Type::sType, POINTER, offsetof(Field, mType), { "type" } },
		{ UwordType, VALUE, offsetof(Field, mKind), { "kind" } },
		{ UwordType, VALUE, offsetof(Field, mOffset), { "offset" } },
		{ String::sType, VALUE, offsetof(Field, mName), { "name" } }
	};

	Array _FieldFieldsArray = {
		Field::sType,
		4,
		&_FieldFields
	};

	Type _FieldType = {
		sizeof(Field),
		sizeof(Uword),
		_FieldFieldsArray
	};

	Type* Field::sType = &_FieldType;

	static void _init(Self* self) {
		Field* f = (Field*) self;
		f->mType = Nothing::sType;
		f->mKind = VALUE;
		f->mOffset = 0;
		f->mName.sObjectFns->init((Self*) &f->mName);
	}

	static void _destroy(Self* self) {
	}

	static Type* mTypeFn(Self* self) {
		return Field::sType;
	}

	static Uword _hash(Self* self) {
		Field* f = (Field*) self;
		Uword hash = 17;
		hash += f->mKind * 37;
		hash += f->mName.sObjectFns->hash((Self*)&f->mName) * 37;
		hash += f->mOffset * 37;
		hash += f->mType->sObjectFns->hash((Self*)f->mType) * 37;
		return hash;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != Field::sType) {
			return False;
		}
		Field* f = (Field*) self;
		Field* otherF = (Field*) other.mSelf;
		return f->mKind == otherF->mKind &&
			f->mName.sObjectFns->equals((Self*)&f->mName, otherF->mName.asObject()) &&
			f->mOffset == otherF->mOffset &&
			f->mType->sObjectFns->equals((Self*)f->mType, otherF->mType->asObject());
	}

	static void _trace(Self* self, MemoryManager mm) {
		Field* f = (Field*) self;
		Uword markResult;
		mm.functions->mark(mm.self, f, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		f->mName.sObjectFns->trace((Self*)&f->mName, mm);
		f->mType->sObjectFns->trace((Self*)f->mType, mm);
	}

	ObjectFunctions _objectFns = { _init, _destroy, mTypeFn, _hash, _equals, _trace };

	ObjectFunctions* Field::sObjectFns = &_objectFns;

	Object Field::asObject() {
		return{ (Self*)this, Field::sObjectFns };
	}

}
