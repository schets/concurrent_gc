#pragma once

class Object;

// A class ((with getters/setters!) now seems silly, but this may eventually
// be a forwarding pointer or something...
class Reference {
	Object* obj;
public:
	Reference(Object *o): obj(o) {}
    Reference(): Reference(nullptr) {}
	Object* get_object() { return obj; }
	const Object* get_object() const { return obj; }
	void set_object(Object* newobj) { obj = newobj; }
	bool is_null() const { return obj == nullptr; }
};

