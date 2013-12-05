#ifndef OCT_TYPE_H
#define OCT_TYPE_H

#include "Array.h"

namespace octarine {

	// Current Type describes only structs.
	// Should probably rename it to StructType or StructInfo.
	// Types of types to add:
	// Variant  - for signaling optional values and ensuring
	//            that they are handled by the programmer.
	// Protocol - describes an extensible interface for virtual
	//            function call dispatch.
	// Function - describes a function signature. This includes
	//            all parameters, return values and the name.
	// VTable   - virtual call dispatch table used in protocol
	//            implementations. Just a list of function
	//            pointers.
	// These are all different kinds of type info so Type needs
	// to be polymorphic somehow.
	// Type should probably be a Variant that can be one of the
	// types above as well as the current type (StructType/StructInfo)
	//
	// OR: Perhaps everything can be implemented using protocols!
	// Instead of explicit data members, there are getter functions
	// for the values... but then - what are those values? Do only
	// primitive machine values exist? And how do we interact with C?
	// Perhaps a bad idea.

	struct Type {
		Uword mSize;
		Uword mAlignment;
		Array mFields;
		ObjectFunctions* mObjectFns;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

}

#endif
