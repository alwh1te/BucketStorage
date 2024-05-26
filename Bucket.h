//
// Created by Alexander on 26.05.2024.
//

#pragma once

#ifndef BUCKETSTORAGE_BUCKET_H
#define BUCKETSTORAGE_BUCKET_H

//#include <iostream>
#include <stdexcept>

template<typename T>
class Bucket {
private:
    T* elements;
    bool* active;
    size_t capacity;
    size_t active_count;

public:
    explicit Bucket(size_t size = 64) {
        elements = new T[size];
        active = new bool[size];
        capacity = size;
        active_count = 0;
    }
    ~Bucket() {
        delete[] elements;
        delete[] active;
    }

    // Метод для добавления элемента в ведро
    void add(const T& item) {
        if (active_count < capacity) {
            elements[active_count] = item;
            active[active_count] = true;
            active_count++;
        } else {
            std::cout << "Bucket is full." << std::endl;
        }
    }

    // Метод для удаления элемента из ведра
    void remove(const T& item) {
        for (size_t i = 0; i < active_count; i++) {
            if (elements[i] == item && active[i]) {
                active[i] = false;
                active_count--;
                return;
            }
        }
        std::cout << "Element not found in the bucket." << std::endl;
    }

    // Метод для получения количества активных элементов в ведре
    size_t size() const {
        return active_count;
    }

    // Метод для проверки, содержится ли элемент в ведре
    bool contains(const T& item) const {
        for (size_t i = 0; i < active_count; i++) {
            if (elements[i] == item && active[i]) {
                return true;
            }
        }
        return false;
    }

    // Итератор для перебора активных элементов в ведре
    class Iterator {
    private:
        T* ptr;
        bool* active;
        size_t count;

    public:
        Iterator(T* elements, bool* active, size_t count)
            : ptr(elements), active(active), count(count) {}

        T& operator*() const {
            return *ptr;
        }

        Iterator& operator++() {
            do {
                ptr++;
                count--;
            } while (!active[count]);

            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
    };

    Iterator begin() {
        return Iterator(elements, active, active_count);
    }

    Iterator end() {
        return Iterator(elements + capacity, active, 0);
    }
};

#endif//BUCKETSTORAGE_BUCKET_H
