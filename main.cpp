#include "Container.h"
#include "LinkedList.h"
#include "bucket_storage.hpp"
#include "helpers.h"
#include <iostream>
using namespace std;


int main() {
    //    LinkedList<int> list;
    //    for (int i = 0; i < 5; ++i) {
    //        list.put(i);
    //    }

    //    for (auto it : list) {
    //        cout << it << endl;
    //    }

    //        Bucket<int> bucket(20);
    //
    //        for (int i = 0; i < 5; ++i) {
    //            bucket.insert(i + 10);
    //        }
    //
    //        for (int i = 0; i < 5; ++i) {
    //            cout << bucket[i] << endl;
    //        }
    //        bucket.printActive();

    //    BucketStorage<int> storage(1, 10);
    //    for (int i = 0; i < 5; ++i) {
    //        storage.add(i);
    //    }
    //
    //    for (int i = 0; i < 5; ++i) {
    //        cout << storage.get(i) << endl;
    //    }

    //    cout << storage.getLastBucket() << endl;

    //    Vector<int> vector;
    //    for (int i = 0; i < 5; ++i) {
    //        vector.push_back(i);
    ////        cout << vector.element() << endl;
    //    }
    //
    //    for (int i = 0; i < 5; ++i) {
    //        cout << vector.front() << endl;
    //        vector.pop_back();
    //    }

    BucketStorage<CountedOperationObject> storage;
    CountedOperationObject el = CountedOperationObject(1);

    cout << storage.empty() << endl;

    cout << (storage.begin() == storage.end()) << endl;

    storage.insert(el);

    cout << storage.empty() << endl;

    cout << storage.capacity() << endl;

    cout << (storage.begin() == storage.end()) << endl;

    cout << opCount.creationCount << endl;

    //    for (int i = 0; i < 5; ++i) {
    //        storage.insert(i);
    //    }
    //
    //    while (!storage.empty()) {
    //        cout << *storage.begin() << endl;
    //        auto it = storage.erase(storage.begin());
    //        cout << "--------" << endl;
    //        if (it != storage.end())
    //            cout << *it << endl;
    //    }
    return 0;
}