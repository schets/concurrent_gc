#include "GC.h"
#include <random>
#include <atomic>
#include <chrono>
#include <iostream>
GC global_gc;
size_t runs = 0;
constexpr size_t num_roots = 16;
constexpr size_t n_refs = 4;

constexpr size_t nevery = 4096*4096/32;

constexpr size_t kill_val = 80;
constexpr size_t depth_chance = 950;

using TimeT = std::chrono::milliseconds;
template<typename F>
static typename TimeT::rep execution(F const &func)
{
    auto start = std::chrono::system_clock::now();
    func();
    auto duration = std::chrono::duration_cast< TimeT>(
        std::chrono::system_clock::now() - start);
    return duration.count();
}

std::vector<Reference> roots;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<size_t> dis(0, 1000);
int main() {
    // Generate roots
    for (size_t i = 0; i < num_roots; i++) {
        roots.push_back(global_gc.create<RefObject<n_refs>>());
    }

    size_t done = 0;
    // now mutate!!

    for (size_t i = 0; i < 10000000000; i++) {
        // transverse the graph

        // select starting root
        size_t root = dis(gen) % num_roots;
        Reference cur = roots[root];
        if (cur.is_null()) {
            roots[root] = global_gc.create<RefObject<n_refs>>();
            continue;
        }

        // now do the Mutator
        for (;;) {

            size_t do_val = dis(gen);
            if (do_val >= depth_chance) {
                break;
            }

            size_t ref_index = do_val % n_refs;
            Reference& next = (*cur.get_object())[ref_index];

            if (next.is_null() || do_val <= kill_val) {
               next = global_gc.create<RefObject<n_refs>>();
               break;
            }

            cur = next;
        }
        ++done;
        if (done % nevery == 0) {
            std::cout << "GC took " << execution([] () {global_gc.run_gc(roots); }) << std::endl;
        }
    }
    for (Reference& r : roots) {
        r.set_object(nullptr);
    }
    global_gc.run_gc(roots);
}
