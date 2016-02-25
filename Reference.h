#pragma once

class Object;

// A class ((with getters/setters!) now seems silly, but this may eventually
// be a forwarding pointer or something...
class Reference {
	Object* obj;
public:
	Object* get_object() { return obj; }
	const Object* get_object() const { return obj; }
	void set_object(Object* newobj) { obj = newobj; }
};
