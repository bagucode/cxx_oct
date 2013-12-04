#include "String.h"
#include "Field.h"
#include "Type.h"

#include <stddef.h>
#include <memory>

namespace octarine {

	Field _StringFields[] = {
		{ UwordType, VALUE, offsetof(String, mSize), { 4, "size" } },
		{ AddressType, VALUE, offsetof(String, mData), { 4, "data" } }
	};

	Array _StringFieldsArray = {
		Field::sType,
		VALUE,
		2,
		&_StringFields
	};

	Type _StringType = {
		sizeof(String),
		sizeof(Uword),
		_StringFieldsArray
	};

	Type* String::sType = &_StringType;

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
		mm.functions->mark(mm.self, (Address)s->mData, &markResult);
	}

	ObjectFunctions _objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	ObjectFunctions* String::sObjectFns = &_objectFns;

	Object String::asObject() {
		return{ (Self*)this, String::sObjectFns };
	}

}
