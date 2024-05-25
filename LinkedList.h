//
// Created by Alexander on 26.05.2024.
//
#pragma once

#ifndef BUCKETSTORAGE_LINKEDLIST_H
#define BUCKETSTORAGE_LINKEDLIST_H

#include <stdexcept>

template <class T>
class Node {
public:
    T data;
    Node * prev;
    Node * next;

    explicit Node(T value) : data(value), prev(nullptr), next(nullptr) {}
};

template <class T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    void insertAtBeginning(T value) {
        auto* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void insertAtEnd(T value) {
        auto* newNode = new Node<T>(value);
        if (tail == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    T getAt(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        auto* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }
    class Iterator {
    private:
        Node<T>* current;

    public:
        explicit Iterator(LinkedList<T>* node) : current(node) {}

        T& operator*() {
            return current->data;
        }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};


#endif//BUCKETSTORAGE_LINKEDLIST_H
