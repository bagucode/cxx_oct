#include "Type.h"
#include "Field.h"
#include "Array.h"
#include "Nothing.h"
#include <stddef.h>

namespace octarine {

	Field _TypeFields [] = {
		{ UwordType, VALUE, offsetof(Type, mSize), { "size" } },
		{ UwordType, VALUE, offsetof(Type, mAlignment), { "alignment" } },
		{ Array::type, VALUE, offsetof(Type, mFields), { "fields" } },
		{ ObjectFunctions::sType, POINTER, offsetof(Type, mObjectFns), { "object-fns" } }
	};

	Array _TypeFieldsArray = {
		Field::sType,
		4,
		&_TypeFields
	};

	Type _TypeType = {
		sizeof(Type),
		sizeof(Uword),
		_TypeFieldsArray
	};

	Type* Type::sType = &_TypeType;

	static void _init(Self* self) {
		Type* t = (Type*) self;
		t->mAlignment = 0;
		t->mFields.objectFns->init((Self*) &t->mFields);
		t->mSize = 0;
		t->mObjectFns = Nothing::sObjectFns; // This could be a very bad idea...
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return Type::sType;
	}

	static Uword _hash(Self* self) {
		Type* t = (Type*) self;
		Uword hash = 17;
		hash += t->mSize * 37;
		hash += t->mAlignment * 37;
		return hash + t->mFields.objectFns->hash((Self*) &t->mFields) * 37;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != Type::sType) {
			return False;
		}
		Type* t = (Type*) self;
		Type* otherT = (Type*) other.mSelf;
		return t->mSize == otherT->mSize &&
			t->mAlignment == otherT->mAlignment &&
			t->mFields.objectFns->equals((Self*)&t->mFields, otherT->mFields.asObject());
	}

	static void _trace(Self* self, MemoryManager mm) {
		Type* t = (Type*) self;
		Uword markResult;
		mm.functions->mark(mm.self, t, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		t->mFields.objectFns->trace((Self*) &t->mFields, mm);
	}

	ObjectFunctions _objectFns = { _init, _destroy, _type, _hash, _equals, _trace };
	
	ObjectFunctions* Type::sObjectFns = &_objectFns;

	Object Type::asObject() {
		return{ (Self*)this, Type::sObjectFns };
	}

}
