#pragma once

#ifndef BUCKETSTORAGE_CONTAINER_H
#define BUCKETSTORAGE_CONTAINER_H
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "LinkedList.h"
#include <iostream>
#include "Vector.h"

//#include "Bucket.h"


template<typename T>
class Container {
private:
    Vector<Bucket<T>> blocks; // Вектор для хранения блоков

public:
    // Метод для вставки элемента
    void insert(const T& object) {
        bool inserted = false;
        for (size_t i = 0; i < blocks.size_(); ++i) {
            if (!blocks[i].full()) {
                blocks[i].insert(object);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            blocks.emplace_back(Bucket<T>(5));
            blocks.back().insert(object);
        }
    }

    // Метод для удаления элемента
    void remove(const T& object) {
        for (auto& block : blocks) {
            if (block.contains(object)) { // Если блок содержит нужный объект
                block.remove(object); // Удаляем объект из блока
                if (block.empty()) { // Если блок стал пустым после удаления
                    blocks.erase(std::remove(blocks.begin(), blocks.end(), block), blocks.end()); // Удаляем блок из контейнера
                }
                return;
            }
        }
    }

    // Метод для вывода всех активных элементов в контейнере
    void printActive() const {
        for (const auto& block : blocks) {
            block.printActive(); // Выводим активные элементы в каждом блоке
        }
    }
};
#endif//BUCKETSTORAGE_CONTAINER_H
