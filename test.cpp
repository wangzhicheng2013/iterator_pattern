#include "vector_container.hpp"
#include "generalized_table.hpp"
#include <iostream>
int main() {
    std::unique_ptr<container<int>>container_ptr(new vector_container<int>);
    for (int i = 0;i < 10;i++) {
        container_ptr->push(i);
    }
    auto si = container_ptr->make_iterator();
    if (!si) {
        return -1;
    }
    while (true == si->has_next()) {
        std::cout << si->get_next() << std::endl;
    }
    container_ptr.reset(new generalize_table);
    const char* str = "(1,2,[3,[4,5]],6)";
    if (false == ((generalize_table*)container_ptr.get())->init(str)) {
        return -2;
    }
    si = container_ptr->make_iterator();
    if (!si) {
        return -3;
    }
    while (true == si->has_next()) {
        std::cout << si->get_next() << std::endl;
    }

    return 0;
}