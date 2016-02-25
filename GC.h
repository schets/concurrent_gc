#pragma once
#include "Reference.h"
#include "Object.h"
#include <vector>
class GC {
	uint8_t colorset;
	Reference head;
	void* alloc_mem(size_t size);
	void free_mem(Reference r);
	void sweep();
public:
	template<class O>
	Reference create() {
		O* ptr = alloc_mem(sizeof(O));
		new (ptr) O(head);
		Object* rval = ptr;
		rval->color = 2;
		rval->next = head;
		head = Reference(rval);
		return rval;
	}
	void run_gc(const std::vector<Reference>& roots);
};

