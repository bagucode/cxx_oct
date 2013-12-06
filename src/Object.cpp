#include "Object.h"
#include "Field.h"
#include "Type.h"
#include "MemoryManager.h"

#include <stddef.h>
#include <memory>

namespace octarine {

	// Object

	Field _ObjectFields[] = {
		{ AddressType, VALUE, offsetof(Object, mSelf), { 4, "self" } },
		{ ObjectFunctions::sType, POINTER, offsetof(Object, mFunctions), { 9, "functions" } }
	};

	Array _ObjectFieldsArray = {
		Field::sType,
		VALUE,
		2,
		&_ObjectFields
	};

	Type _ObjectType = {
		sizeof(Object),
		sizeof(Uword),
		_ObjectFieldsArray
	};

	Type* Object::sType = &_ObjectType;

	static void _init(Self* self) {
		Object* o = (Object*) self;
		// TODO: init to nil instead? (global Nothing instance)
		o->mSelf = nullptr;
		o->mFunctions = nullptr;
	}

	static void _destroy(Self* self) {
	}

	static Type* _type(Self* self) {
		return Object::sType;
	}

	static Uword _hash(Self* self) {
		Object* o = (Object*) self;
		Uword hash = 17;
		hash += ((Uword)o->mSelf) * 37;
		hash += ((Uword) o->mFunctions) * 37;
		return hash;
	}

	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != Object::sType) {
			return False;
		}
		Object* o = (Object*) self;
		Object* otherO = (Object*) other.mSelf; // it's an Object-Object!
		return o->mFunctions == otherO->mFunctions &&
			o->mSelf == otherO->mSelf;
	}

	static void _trace(Self* self, MemoryManager* mm) {
		Object* o = (Object*) self;
		Uword markResult;
		mm->mFunctions->mark(mm->mSelf, o, &markResult);
		if (markResult == MemoryManagerMarkResult::ALREADY_MARKED) {
			return;
		}
		o->mFunctions->trace(o->mSelf, mm);
		o->mFunctions->sObjectFns->trace((Self*) o->mFunctions, mm);
	}

	ObjectFunctions _objectFns = { _init, _destroy, _type, _hash, _equals, _trace };

	ObjectFunctions* Object::sObjectFns = &_objectFns;

	Object Object::asObject() {
		return{ (Self*)this, Object::sObjectFns };
	}

	// ObjectFunctions

	Field _ObjectFunctionsFields[] = {
		{ AddressType, VALUE, offsetof(ObjectFunctions, init), { 4, "init" } },
		{ AddressType, VALUE, offsetof(ObjectFunctions, destroy), { 7, "destroy" } },
		{ AddressType, VALUE, offsetof(ObjectFunctions, type), { 4, "type" } },
		{ AddressType, VALUE, offsetof(ObjectFunctions, hash), { 4, "hash" } },
		{ AddressType, VALUE, offsetof(ObjectFunctions, equals), { 6, "equals" } },
		{ AddressType, VALUE, offsetof(ObjectFunctions, trace), { 5, "trace" } }
	};

	Array _ObjectFunctionsFieldsArray = {
		Field::sType,
		VALUE,
		6,
		&_ObjectFunctionsFields
	};

	Type _ObjectFunctionsType = {
		sizeof(ObjectFunctions),
		sizeof(Uword),
		_ObjectFunctionsFieldsArray
	};

	Type* ObjectFunctions::sType = &_ObjectFunctionsType;

	static void _initF(Self* self) {
		ObjectFunctions* of = (ObjectFunctions*) self;
		memset(of, 0, sizeof(ObjectFunctions));
	}

	static void _destroyF(Self* self) {
	}

	static Type* _typeF(Self* self) {
		return ObjectFunctions::sType;
	}

	static Uword _hashF(Self* self) {
		ObjectFunctions* of = (ObjectFunctions*) self;
		Uword hash = 17;
		for (Uword i = 0; i < sizeof(ObjectFunctions) / sizeof(Uword); ++i) {
			hash += ((Uword*) of)[i] * 37;
		}
		return hash;
	}

	static Bool _equalsF(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != ObjectFunctions::sType) {
			return False;
		}
		ObjectFunctions* of = (ObjectFunctions*) self;
		ObjectFunctions* otherOF = (ObjectFunctions*) other.mSelf;
		return memcmp(of, otherOF, sizeof(ObjectFunctions));
	}

	static void _traceF(Self* self, MemoryManager mm) {
		ObjectFunctions* of = (ObjectFunctions*) self;
		Uword markResult;
		mm.functions->mark(mm.self, of, &markResult);
	}

	ObjectFunctions _objectFunctionsFns = { _initF, _destroyF, _typeF, _hashF, _equalsF, _traceF };

	ObjectFunctions* ObjectFunctions::sObjectFns = &_objectFunctionsFns;

	Object ObjectFunctions::asObject() {
		return{ (Self*)this, ObjectFunctions::sObjectFns };
	}
}
