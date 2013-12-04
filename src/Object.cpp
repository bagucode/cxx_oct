#include "Object.h"

#include <stddef.h>

namespace octarine {

	Field _ObjectFields[] = {
		{ UwordType, VALUE, offsetof(String, mSize), { 4, "size" } },
		{ AddressType, VALUE, offsetof(String, mData), { 4, "data" } }
	};

	Array _ObjectFieldsArray = {
		Field::sType,
		VALUE,
		2,
		&_ObjectFields
	};

	Type _ObjectType = {
		sizeof(String),
		sizeof(Uword),
		_ObjectFieldsArray
	};

	Type* String::sType = &_ObjectType;

	static void _init(Self* self) {
		String* s = (String*) self;
		s->mData = "";
		s->mSize = 0;
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return String::sType;
	}

	static Uword _hash(Self* self) {
		String* s = (String*) self;
		Uword hash = 17;
		hash += s->mSize * 37;
		for (Uword i = 0; i < s->mSize; ++i) {
			hash += s->mData[i] * 37;
		}
		return hash;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != String::sType) {
			return False;
		}
		String* s = (String*) self;
		String* otherS = (String*) other.mSelf;
		if (s->mSize != otherS->mSize) {
			return False;
		}
		return memcmp(s->mData, otherS->mData, s->mSize) == 0;
	}

	static void _trace(Self* self, MemoryManager mm) {
		String* s = (String*) self;
		Uword markResult;
		mm.functions->mark(mm.self, s, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		mm.functions->mark(mm.self, (Address) s->mData, &markResult);
	}

	ObjectFunctions _objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	ObjectFunctions* String::sObjectFns = &_objectFns;

	Object String::asObject() {
		return{ (Self*)this, String::sObjectFns };
	}

}
