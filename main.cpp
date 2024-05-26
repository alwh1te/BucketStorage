#include "Container.h"
#include <iostream>

int main() {

    Container<int> storage{};
    //    storage.insert(1);
    //    storage.insert(2);
    for (int i = 0; i < 5; ++i) {
        storage.addElement(i);
    }
    for (auto it = storage.begin(); it != storage.end(); it++) {
        std::cout << *it << " ";
    }
//    auto it = storage.begin();
//    storage.end();
    //        it++;
    //    std::cout << *it << " ";

//    for (int count = 0; count < 100; ++count) {
//        std::cout << *it << " ";
//        it++;
//    }
    std::cout << std::endl;


    return 0;
}
