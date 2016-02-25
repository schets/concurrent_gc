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
	virtual ~Object() = 0;
	virtual ref_list get_references() = 0;
	virtual size_t get_size() = 0;
};

// N is # of bytes in object, no references
template<uint16_t N>
class SizedObject : public virtual Object {
	constexpr static uint16_t rsize = N == 0 ? 1 : N;
	static_assert(N <= 2048, "SizedObject must be less than 2048 bytes");
	char blob[rsize];
public:
	override ref_list get_references() { return{ nullptr, 0 }; }
	override size_t get_size() { return sizeof(*this); }
};

// N is the number of references this has!
template<uint8_t N>
class RefObject : public virtual Object {
	Reference[N] refs;
public:
	override ref_list get_references() { return{ refs, N }; }
	override size_t get_size() { return sizeof(*this); }
};

template<uint8_t NR, uint16_t NB>
class SizedRefObject : public RefObject {
	SizedObject space<NB>;
public:
	override size_t get_size() { return sizeof(*this); }
};