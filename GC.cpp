#include "GC.h"

#include <stdlib.h>

// white = unseen
// grey = seen but partially marked
// black = seen and fully marked

struct colors {
	uint8_t white;
	uint8_t black;
};

colors colorsets[] = { {1, 2}, {2, 1} };

void traverse(Reference r, const colors& current_colors);
Reference sweep(Reference list_head);

// More advanced mechanisms (compacting, generational) to come
void* GC::alloc_mem(size_t s) {
	return malloc(s);
}

void GC::free_mem(Reference r) {
	free(r.get_object());
}

uint8_t GC::get_starting_color() {
	return colorsets[colorset].white;
}

void GC::run_gc(const std::vector<Reference>& roots) {
	colors current_colors = colorsets[colorset];

	// yay graph traversal
	for (Reference r : roots) {
		traverse(r, current_colors);
	}
	sweep();
	// swap colorset
	//colorset ^= 1;
}

// Could optimize stack usage here, can already know 'white'/'black'
// from passed object
void traverse(Reference r, const colors& current_colors) {
	if (r.is_null()) {
		return;
	}
	Object* obj = r.get_object();
	uint8_t cur_color = obj->color;
	if (cur_color != current_colors.white) {
		return;
	}

	if (cur_color == current_colors.white) {
		obj->color = current_colors.black;
	}

	ref_list references = obj->get_references();

	for (size_t i = 0; i < references.size; i++) {
		traverse(references.references[i], current_colors);
	}
}

void GC::sweep() {
	if (head.is_null()) { return; }

	uint8_t good_color = colorsets[colorset].black;
	uint8_t fresh_color = colorsets[colorset].white;
	Reference new_head = head;
	while (!new_head.is_null() &&
		   new_head.get_object()->color != good_color) {
		free_mem(new_head);
		new_head = new_head.get_object()->next;
	}

	head = new_head;
	head.get_object()->color = fresh_color;
	Reference prev = head;
	Reference next = head.get_object()->next;
	while (!next.is_null()) {
		Object* obj = next.get_object();
		if (obj->color == good_color) {
			obj->color = fresh_color;
			prev = next;
			next = obj->next;
		}
		else {
			Reference tofree = next;
			next = obj->next;
			free_mem(tofree);
		}
	}
}
