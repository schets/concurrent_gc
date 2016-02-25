#pragma once
#include <atomic>
class Object;

// A class ((with getters/setters!) now seems silly, but this may eventually
// be a forwarding pointer or something...
struct Reference {
    std::atomic<Object*> obj;
	Reference(Object *o): obj(o) {}
    Reference(): Reference(nullptr) {}
    Reference(const Reference& r): obj(r.obj.load(std::memory_order_relaxed)) {}
    Reference& operator=(const Reference& o) {
        obj.store(o.obj.load(std::memory_order_relaxed), std::memory_order_relaxed);
        return *this;
    }
    ~Reference() {}
	Object* get_object() { return obj; }
	const Object* get_object() const { return obj.load(std::memory_order_relaxed); }
	void set_object(Object* newobj) { obj.store(newobj, std::memory_order_relaxed); }
	bool is_null() const { return get_object() == nullptr; }
    void safe_transfer(volatile Reference& to) {
    }
};
