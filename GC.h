#pragma once
#include "Reference.h"
#include "Object.h"
#include <vector>
class GC {
	uint8_t colorset;
	Reference head;
	void* alloc_mem(size_t size);
	void free_mem(Reference r);
	uint8_t get_starting_color();
	void sweep();
public:
	GC(): colorset(0), head(nullptr) {}
	template<class O>
	Reference create() {
		O* ptr = (O*)alloc_mem(sizeof(O));
		new (ptr) O();
		Object* rval = ptr;
		rval->color = get_starting_color();
		rval->next = head;
		head = Reference(rval);
		return rval;
	}
	void run_gc(const std::vector<Reference>& roots);
};
