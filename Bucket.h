//
// Created by Alexander on 26.05.2024.
//

#pragma once

#ifndef BUCKETSTORAGE_BUCKET_H
#define BUCKETSTORAGE_BUCKET_H

//#include <iostream>
#include <stdexcept>
#include "Vector.h"

template <typename T>
class Bucket {
private:
    Vector<T> objects; // Вектор для хранения объектов в блоке
    Vector<int> statuses; // Вектор для отслеживания статусов каждого места в блоке

public:
    // Конструктор блока
    explicit Bucket(size_t size) : objects(size), statuses(size) {} // Инициализация векторов нужного размера

    // Метод для вставки элемента
    void insert(const T& object) {
        for (size_t i = 0; i < objects.size(); ++i) {
            if (statuses[i] == 0) { // Если место свободно
                objects[i] = object; // Вставляем объект
                statuses[i] = 1; // Меняем статус на "занято"
                return;
            }
        }
    }

    // Метод для удаления элемента
    void remove(T& object) {
        for (size_t i = 0; i < objects.size(); ++i) {
            if (statuses[i] && objects[i] == object) { // Если место занято и содержит нужный объект
                objects[i].~T();
                statuses[i] = 0; // Меняем статус на "свободно"
                return;
            }
        }
    }

    // Метод для вывода всех активных элементов
    void printActive() {
        for (size_t i = 0; i < objects.size(); ++i) {
            if (statuses[i] > 0) { // Если место занято
                std::cout << objects[i] << " "; // Выводим объект
            }
        }
        std::cout << std::endl;
    }
};
#endif//BUCKETSTORAGE_BUCKET_H
