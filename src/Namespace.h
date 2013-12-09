#ifndef OCT_NAMESPACE_H
#define OCT_NAMESPACE_H

#include "String.h"
#include "Array.h"

namespace octarine {

	struct NamespaceEntry {
		String name;
		Object value;

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

	struct Namespace {
		String name;
		Array entries; // TODO: Use a map.

		static Type* sType;
		static ObjectFunctions* sObjectFns;
		Object asObject();
	};

	/*
    Namespace createNamespace(Runtime rt, String name);
    
    Object bind(ThreadContext tc, Namespace ns, String name, Object obj);
    
    Type getNamespaceType(Runtime rt);
    
    Bool equals(ThreadContext tc, Namespace ns, Object obj);
	*/

}

#endif
