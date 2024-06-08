#include "Bucket.h"
#include "BucketStorage.h"
#include "Container.h"
#include "LinkedList.h"
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

        Bucket<int> bucket(20);

        for (int i = 0; i < 5; ++i) {
            bucket.insert(i + 10);
        }

//        for (int i = 0; i < 5; ++i) {
//            cout << bucket[i] << endl;
//        }
        bucket.printActive();

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
//        vector.enqueue(i);
////        cout << vector.element() << endl;
//    }
//
//    for (int i = 0; i < 5; ++i) {
//        cout << vector.dequeue() << endl;
//    }

    return 0;
}