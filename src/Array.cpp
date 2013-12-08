#include "Array.h"
#include "Field.h"
#include "Type.h"
#include "Nothing.h"
#include "MemoryManager.h"
#include <stddef.h>

namespace octarine {

	Field _ArrayFields[] = {
		{ Type::sType, POINTER, offsetof(Array, mType), { 4, "type" } },
		{ UwordType, VALUE, offsetof(Array, mKind), { 4, "kind" } },
		{ UwordType, VALUE, offsetof(Array, mSize), { 4, "size" } },
		{ AddressType, VALUE, offsetof(Array, mData), { 4, "data" } }
	};

	Array _ArrayFieldsArray = {
		Field::sType,
		VALUE,
		4,
		&_ArrayFields
	};

	Type _ArrayType = {
		sizeof(Array),
		sizeof(Uword),
		_ArrayFieldsArray
	};

	Type* Array::sType = &_ArrayType;

	static void _init(Self* self) {
		Array* a = (Array*) self;
		a->mData = nullptr;
		a->mSize = 0;
		a->mKind = VALUE;
		a->mType = Nothing::sType;
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return Array::sType;
	}

	static Uword _hash(Self* self) {
		Array* a = (Array*) self;
		Uword hash = 17;
		hash += a->mSize * 37;
		hash += a->mKind * 37;
		hash += a->mType->sObjectFns->hash((Self*)a->mType) * 37;
		for (Uword i = 0; i < a->mSize; ++i) {
			Self* arrayField;
			if (a->mKind == VALUE) {
				arrayField = (Self*) ((U8*) a->mData + i * a->mType->mSize);
			}
			else {
				arrayField = *((Self**) ((U8*) a->mData + i * sizeof(Address)));
			}
			hash += a->mType->mObjectFns->hash((Self*) arrayField) * 37;
		}
		return hash;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != Array::sType) {
			return False;
		}
		Array* a = (Array*) self;
		Array* otherA = (Array*) other.mSelf;
		if (!(a->mKind == otherA->mKind &&
			a->mSize == otherA->mSize &&
			a->mType->sObjectFns->equals((Self*) a->mType, otherA->mType->asObject()))) {
			return False;
		}
		for (Uword i = 0; i < a->mSize; ++i) {
			Self* arrayField1;
			Self* arrayField2;
			if (a->mKind == VALUE) {
				arrayField1 = (Self*) ((U8*) a->mData + i * a->mType->mSize);
				arrayField2 = (Self*) ((U8*) otherA->mData + i * a->mType->mSize);
			}
			else {
				arrayField1 = *((Self**) ((U8*) a->mData + i * sizeof(Address)));
				arrayField2 = (Self*) ((U8*) otherA->mData + i * a->mType->mSize);
			}
			// TODO: This conversion to Object should probably be in a function somewhere?
			Object arrayField2AsObject;
			arrayField2AsObject.mSelf = arrayField2;
			arrayField2AsObject.mFunctions = a->mType->mObjectFns;
			if (!a->mType->mObjectFns->equals(arrayField1, arrayField2AsObject)) {
				return False;
			}
		}
		return True;
	}

	static void _trace(Self* self, MemoryManager* mm) {
		Array* a = (Array*) self;
		Uword markResult;
		mm->mFunctions->mark(mm->mSelf, a, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		a->mType->sObjectFns->trace((Self*) a->mType, mm);
		for (Uword i = 0; i < a->mSize; ++i) {
			Self* arrayField;
			if (a->mKind == VALUE) {
				arrayField = (Self*) ((U8*) a->mData + i * a->mType->mSize);
			}
			else {
				arrayField = *((Self**) ((U8*) a->mData + i * sizeof(Address)));
			}
			a->mType->mObjectFns->trace(arrayField, mm);
		}
	}

	ObjectFunctions _objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	ObjectFunctions* Array::sObjectFns = &_objectFns;

	Object Array::asObject() {
		return{ (Self*)this, Array::sObjectFns };
	}

}
