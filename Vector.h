//
// Created by Alexander on 01.06.2024.
//

#ifndef BUCKETSTORAGE_VECTOR_H
#define BUCKETSTORAGE_VECTOR_H

#include <iostream>
template<typename T>
class Vector {
private:
    T* elements;
    int head;
    int tail;
    int size_;
    size_t capacity_;

public:
    explicit Vector(size_t size) : elements(new T[size]), head(0), tail(0), size_(0), capacity_(size) {}

    explicit Vector() : elements(new T[2]), head(0), tail(0), size_(0), capacity_(2) {}

    ~Vector() {
        delete[] elements;
    }

    void enqueue(const T& element) {
        if (size_ == capacity_) {
//            reserve(capacity_ * 2);
            ensureCapacity();
        }
        elements[tail] = element;
        tail = (tail + 1) % capacity_;
        ++size_;
    }

    T element() const {
        return elements[head];
    }

    T dequeue() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        T temp = elements[head];
        elements[head] = T(); // Or any default value for T
        head = (head + 1) % capacity_;
        --size_;
        return temp;
    }

    void clear() {
        fillArray(elements + min(tail, head), elements + max(tail, head), T());
        size_ = 0;
        head = 0;
        tail = 0;
    }

    size_t size() {
        return size_;
    }

    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

private:
    size_t capacity() const {
        return tail < head ? (tail + capacity_) : capacity_;
    }

    void ensureCapacity() {
        if (size_ == capacity_) {
            int newCapacity = capacity_ * 2;
            T* newArray = new T[newCapacity];
            copyArray(elements + head, elements + capacity_, newArray);
            copyArray(elements, elements + tail, newArray + capacity_ - head);
            delete[] elements;
            elements = newArray;
            head = 0;
            tail = size_;
            capacity_ = newCapacity;
        }
    }


    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity_) return;

        T* newArray = new T[newCapacity];
        copyArray(elements + head, elements + capacity_, newArray);
        copyArray(elements, elements + tail, newArray + capacity_ - head);
        delete[] elements;
        elements = newArray;
        head = 0;
        tail = size_;
        capacity_ = newCapacity;
    }

    static void copyArray(const T* sourceStart, const T* sourceEnd, T* dest) {
        while (sourceStart != sourceEnd) {
            *dest++ = *sourceStart++;
        }
    }

    static void fillArray(T* start, T* end, const T& value) {
        while (start != end) {
            *start++ = value;
        }
    }

    static int min(int a, int b) {
        return a < b ? a : b;
    }

    static int max(int a, int b) {
        return a > b ? a : b;
    }
};


#endif//BUCKETSTORAGE_VECTOR_H
