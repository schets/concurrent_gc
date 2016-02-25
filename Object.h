#pragma once

#include "Reference.h"
#include <stdint.h>
#include <stddef.h>
// I don't feel like making an interpreter and VM with real classes,
// so I'm just gonna dealing with the GC over the graph part of things

struct ref_list {
	Reference* references;
	uint8_t size;
};

class Object {
public:
	Reference next;
	uint8_t color;
	virtual ~Object() {}
	virtual ref_list get_references() = 0;
	virtual size_t get_size() = 0;
};

// N is the number of references this has!
template<uint8_t N>
class RefObject : public Object {
	static_assert(N > 0, "RefObject must have nonzero references");
	Reference refs[N];
public:
	RefObject() {
		for (size_t i = 0; i < N; i++) {
			refs[i].set_object(nullptr);
		}
	}
	ref_list get_references() override final { return{ refs, N }; }
	size_t get_size() override final { return sizeof(*this); }
	Reference& operator[](size_t index) {
		return refs[index];
	}
	const Reference& operator[](size_t index) const {
		return refs[index];
	}
};
