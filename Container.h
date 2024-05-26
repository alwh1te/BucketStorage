//
// Created by Alexander on 19.05.2024.
//
#pragma once

#ifndef BUCKETSTORAGE_CONTAINER_H
#define BUCKETSTORAGE_CONTAINER_H

//#include <iterator>
//#include <memory>
#include "LinkedList.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "Bucket.h"

template<typename T>
class Container {
private:
    const size_t bucketSize;
    LinkedList<Bucket<T>> buckets;

public:
    explicit Container(size_t size = 64) : bucketSize(size) {
//        Bucket<T> newBucket(bucketSize);
//        buckets.insert(newBucket);
    }

    void addElement(const T &element) {
//        for (auto &bucket: buckets) {
        for (Iterator it = buckets.begin(); it != buckets.end(); it++) {
            if (it.size() < bucketSize) {
                it.add(element);
                return;
            }
        }

        Bucket<T> newBucket(bucketSize);
        newBucket.add(element);
        buckets.put(newBucket);
    }
    class Iterator {
    private:
        const Container& container;
        size_t bucketIndex;
        typename Bucket<T>::Iterator bucketIterator;

    public:
        Iterator(const Container& cont, size_t index)
            : container(cont), bucketIndex(index), bucketIterator(cont.buckets[index].begin()) {}

        T& operator*() const {
            return *bucketIterator;
        }

        Iterator& operator++() {
            ++bucketIterator;
            if (bucketIterator == container.buckets[bucketIndex].end()) {
                bucketIndex++;
                if (bucketIndex < container.buckets.size()) {
                    bucketIterator = container.buckets[bucketIndex].begin();
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (bucketIndex != other.bucketIndex) || (bucketIterator != other.bucketIterator);
        }
    };

    Iterator begin() const {
        return Iterator(*this, 0);
    }

    Iterator end() const {
        return Iterator(*this, buckets.size());
    }
};
#endif//BUCKETSTORAGE_CONTAINER_H
