#include "MemoryManager.h"
#include "Field.h"
#include "Type.h"
#include <stddef.h>

namespace octarine {

	Field _MemoryManagerFields[] = {
		{ AddressType, VALUE, offsetof(MemoryManager, mSelf), { 4, "self" } },
		{ ObjectFunctions::sType, POINTER, offsetof(MemoryManager, mFunctions), { 9, "functions" } }
	};
    
	Array _MemoryManagerFieldsArray = {
		Field::sType,
		VALUE,
		2,
		&_MemoryManagerFields
	};
    
	Type _MemoryManagerType = {
		sizeof(MemoryManager),
		sizeof(Uword),
		_MemoryManagerFieldsArray
	};
    
	Type* MemoryManager::sType = &_MemoryManagerType;
    
	static void _init(Self* self) {
        MemoryManager* mm = (MemoryManager*)self;
        mm->mFunctions = nullptr;
        mm->mSelf = nullptr;
	}
    
	static void _destroy(Self* self) {
	}
    
	static Type* _type(Self* self) {
		return MemoryManager::sType;
	}
    
	static Uword _hash(Self* self) {
		MemoryManager* m = (MemoryManager*) self;
		Uword hash = 17;
		hash += ((Uword) m->mSelf) * 37;
		hash += m->mFunctions->sObjectFns->hash((Self*)m->mFunctions) * 37;
		return hash;
	}
    
	static Bool _equals(Self* self, Object other) {
		if (self == other.mSelf) {
			return True;
		}
		if (other.mFunctions->type(other.mSelf) != MemoryManager::sType) {
			return False;
		}
		MemoryManager* m = (MemoryManager*) self;
		MemoryManager* otherM = (MemoryManager*) other.mSelf;
		return m->mSelf == otherM->mSelf &&
            m->mFunctions->sObjectFns->equals((Self*)m->mFunctions, otherM->mFunctions->asObject());
	}
    
	static void _trace(Self* self, MemoryManager* mm) {
		// Yeah.
	}
    
	ObjectFunctions _objectFns = { _type, _init, _destroy, _hash, _equals, _trace };
    
	ObjectFunctions* MemoryManager::sObjectFns = &_objectFns;
    
	Object MemoryManager::asObject() {
		return{ (Self*)this, MemoryManager::sObjectFns };
	}

}
