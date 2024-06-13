#pragma once

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>

// ------------------------------------
// START OF INTERFACE
// ------------------------------------

template<class T>
class BucketStorage {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    class iterator;
    class const_iterator;

    explicit BucketStorage(size_type block_capacity = 64);
    BucketStorage(const BucketStorage &other);
    BucketStorage(BucketStorage &&other) noexcept;
    ~BucketStorage();

    BucketStorage &operator=(const BucketStorage &other);
    BucketStorage &operator=(BucketStorage &&other) noexcept;

    iterator insert(const value_type &value);
    iterator insert(value_type &&value);
    iterator erase(iterator it);

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    void shrink_to_fit();
    void clear();
    void swap(BucketStorage &other) noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    iterator get_to_distance(iterator it, difference_type distance);

private:
    struct Block {
        pointer data;
        bool *active;
        size_type capacity;
        size_type size;

        explicit Block(size_type block_capacity);
        ~Block();
    };

    size_type block_capacity;
    Block **blocks;
    size_type block_count;
    size_type total_size;

    void allocate_new_block();
    void deallocate_block(Block *block);
    friend class iterator;
    friend class const_iterator;
};

template<typename T>
class BucketStorage<T>::iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    iterator();
    iterator(BucketStorage *storage, BucketStorage::Block *block, size_type index);

    reference operator*() const;
    pointer operator->() const;

    //    operator const_iterator() const;

    iterator &operator++();
    iterator operator++(int);
    iterator &operator--();
    iterator operator--(int);

    bool operator<(const iterator &other) const;
    bool operator>(const iterator &other) const;
    bool operator<=(const iterator &other) const;
    bool operator>=(const iterator &other) const;

    bool operator==(const iterator &other) const;
    bool operator!=(const iterator &other) const;

    bool operator==(const const_iterator &other) const;
    bool operator!=(const const_iterator &other) const;

private:
    BucketStorage *storage;
    Block *block;
    size_type index;

    friend class BucketStorage;
};


template<typename T>
class BucketStorage<T>::const_iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    const_iterator();
    const_iterator(const BucketStorage *storage, Block *block, size_type index);
    const_iterator(const iterator &it);

    operator iterator() const;

    reference operator*() const;
    pointer operator->() const;

    const_iterator &operator++();
    const_iterator operator++(int);
    const_iterator &operator--();
    const_iterator operator--(int);

    bool operator==(const const_iterator &other) const;
    bool operator!=(const const_iterator &other) const;

private:
    const BucketStorage *storage;
    Block *block;
    size_type index;

    //    const_iterator(const BucketStorage *storage, Block *block, size_type index);

    friend class BucketStorage;
};

// ------------------------------------
// START OF IMPLEMENTATION
// ------------------------------------

template<typename T>
BucketStorage<T>::const_iterator::const_iterator() : storage(nullptr), block(nullptr), index(0) {}

template<typename T>
BucketStorage<T>::const_iterator::const_iterator(const BucketStorage *storage, Block *block, size_type index)
    : storage(storage), block(block), index(index) {}

template<typename T>
BucketStorage<T>::const_iterator::const_iterator(const iterator &it)
    : storage(it.storage), block(it.block), index(it.index) {}

template<typename T>
BucketStorage<T>::const_iterator::operator iterator() const {
    //    return iterator(storage, block, index);
    return const_iterator(*this);
    //  return iterator(const_cast<BucketStorage*>(storage), block, index);
}

template<typename T>
typename BucketStorage<T>::const_iterator::reference BucketStorage<T>::const_iterator::operator*() const {
    return block->data[index];
}

template<typename T>
typename BucketStorage<T>::const_iterator::pointer BucketStorage<T>::const_iterator::operator->() const {
    return &block->data[index];
}

template<typename T>
typename BucketStorage<T>::const_iterator &BucketStorage<T>::const_iterator::operator++() {
    for (size_type i = index + 1; i < storage->block_capacity; ++i) {
        if (block->active[i]) {
            index = i;
            return *this;
        }
    }

    for (size_type i = 0; i < storage->block_count; ++i) {
        if (storage->blocks[i] == block) {
            for (size_type j = i + 1; j < storage->block_count; ++j) {
                for (size_type k = 0; k < storage->block_capacity; ++k) {
                    if (storage->blocks[j]->active[k]) {
                        block = storage->blocks[j];
                        index = k;
                        return *this;
                    }
                }
            }
            break;
        }
    }

    block = nullptr;
    index = 0;
    return *this;
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::const_iterator::operator++(int) {
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
typename BucketStorage<T>::const_iterator &BucketStorage<T>::const_iterator::operator--() {
    for (difference_type i = index - 1; i >= 0; --i) {
        if (block->active[i]) {
            index = i;
            return *this;
        }
    }

    for (difference_type i = storage->block_count - 1; i >= 0; --i) {
        if (storage->blocks[i] == block) {
            for (difference_type j = i - 1; j >= 0; --j) {
                for (difference_type k = storage->block_capacity - 1; k >= 0; --k) {
                    if (storage->blocks[j]->active[k]) {
                        block = storage->blocks[j];
                        index = k;
                        return *this;
                    }
                }
            }
            break;
        }
    }

    block = nullptr;
    index = 0;
    return *this;
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::const_iterator::operator--(int) {
    const_iterator temp = *this;
    --(*this);
    return temp;
}

template<typename T>
bool BucketStorage<T>::const_iterator::operator==(const const_iterator &other) const {
    return storage == other.storage && block == other.block && index == other.index;
}

template<typename T>
bool BucketStorage<T>::const_iterator::operator!=(const const_iterator &other) const {
    return !(*this == other);
}

template<typename T>
BucketStorage<T>::iterator::iterator() : storage(nullptr), block(nullptr), index(0) {}

template<typename T>
BucketStorage<T>::iterator::iterator(BucketStorage *storage, BucketStorage::Block *block, size_type index)
    : storage(storage), block(block), index(index) {}

template<typename T>
typename BucketStorage<T>::iterator::reference BucketStorage<T>::iterator::operator*() const {
    return block->data[index];
}

template<typename T>
typename BucketStorage<T>::iterator::pointer BucketStorage<T>::iterator::operator->() const {
    return &block->data[index];
}

//template<typename T>
//BucketStorage<T>::iterator::operator const_iterator() const {
//    return iterator(storage, block, index);
//}

template<typename T>
typename BucketStorage<T>::iterator &BucketStorage<T>::iterator::operator++() {
    for (size_type i = index + 1; i < storage->block_capacity; ++i) {
        if (block->active[i]) {
            index = i;
            return *this;
        }
    }

    for (size_type i = 0; i < storage->block_count; ++i) {
        if (storage->blocks[i] == block) {
            for (size_type j = i + 1; j < storage->block_count; ++j) {
                for (size_type k = 0; k < storage->block_capacity; ++k) {
                    if (storage->blocks[j]->active[k]) {
                        block = storage->blocks[j];
                        index = k;
                        return *this;
                    }
                }
            }
            break;
        }
    }

    block = nullptr;
    index = 0;
    return *this;
}


template<typename T>
typename BucketStorage<T>::iterator &BucketStorage<T>::iterator::operator--() {
    for (difference_type i = index - 1; i >= 0; --i) {
        if (block->active[i]) {
            index = i;
            return *this;
        }
    }

    for (difference_type i = storage->block_count - 1; i >= 0; --i) {
        if (storage->blocks[i]) {
            for (difference_type j = i - 1; j >= 0; --j) {
                for (difference_type k = storage->block_capacity - 1; k >= 0; --k) {
                    if (storage->blocks[j]->active[k]) {
                        block = storage->blocks[j];
                        index = k;
                        return *this;
                    }
                }
            }
            break;
        }
    }

    block = nullptr;
    index = 0;
    return *this;
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}

template<typename T>
bool BucketStorage<T>::iterator::operator<(const iterator &other) const {
    return storage == other.storage && block == other.block && index < other.index;
}

template<typename T>
bool BucketStorage<T>::iterator::operator>(const iterator &other) const {
    return !(this < other);
}

template<typename T>
bool BucketStorage<T>::iterator::operator<=(const iterator &other) const {
    return storage == other.storage && block == other.block && index <= other.index;
}

template<typename T>
bool BucketStorage<T>::iterator::operator>=(const iterator &other) const {
    return storage == other.storage && block == other.block && index >= other.index;
}

template<typename T>
bool BucketStorage<T>::iterator::operator==(const iterator &other) const {
    return storage == other.storage && block == other.block && index == other.index;
}

template<typename T>
bool BucketStorage<T>::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}

template<typename T>
bool BucketStorage<T>::iterator::operator==(const const_iterator &other) const {
    return storage == other.storage && block == other.block && index == other.index;
}

template<typename T>
bool BucketStorage<T>::iterator::operator!=(const const_iterator &other) const {
    return storage == other.storage && block == other.block && index != other.index;
}

template<typename T>
BucketStorage<T>::Block::Block(size_type block_capacity)
    : data(static_cast<T *>(::operator new(block_capacity * sizeof(T)))), active(new bool[block_capacity]()), capacity(block_capacity), size(0) {}

template<typename T>
BucketStorage<T>::Block::~Block() {
    //    delete[] data;
    //    delete[] active;
    //    ::operator delete(data);
    for (size_type i = 0; i < size; ++i) {
        //        deallocate_block(blocks[i]);
        data[i].~T();
    }
    delete[] active;
}

template<class T>
BucketStorage<T>::BucketStorage(size_type block_capacity)
    : block_capacity(block_capacity), blocks(nullptr), block_count(0), total_size(0) {
    //    allocate_new_block();
}

template<typename T>
BucketStorage<T>::BucketStorage(const BucketStorage &other)
    : block_capacity(other.block_capacity), blocks(new Block *[other.block_count]), block_count(other.block_count), total_size(other.total_size) {
    for (size_type i = 0; i < block_count; ++i) {
        blocks[i] = new Block(*other.blocks[i]);
    }
}

template<typename T>
BucketStorage<T>::BucketStorage(BucketStorage &&other) noexcept
    : block_capacity(other.block_capacity), blocks(other.blocks), block_count(other.block_count), total_size(other.total_size) {
    other.blocks = nullptr;
    other.block_count = 0;
    other.total_size = 0;
}

template<typename T>
BucketStorage<T>::~BucketStorage() {
    //    for (size_type i = 0; i < block_count; ++i) {
    //        delete blocks[i];
    //    }
    delete[] blocks;
}

template<typename T>
BucketStorage<T> &BucketStorage<T>::operator=(const BucketStorage &other) {
    if (this != &other) {
        BucketStorage tmp(other);
        swap(tmp);
    }
    return *this;
}

template<typename T>
BucketStorage<T> &BucketStorage<T>::operator=(BucketStorage &&other) noexcept {
    if (this != &other) {
        swap(other);
        for (size_type i = 0; i < other.block_count; ++i) {
            delete other.blocks[i];
        }
        delete[] other.blocks;
        other.blocks = nullptr;
        other.block_count = 0;
        other.total_size = 0;
    }
    return *this;
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::insert(const value_type &value) {
    for (size_type i = 0; i < block_count; ++i) {
        if (blocks[i]->size < block_capacity) {
            for (size_type j = 0; j < block_capacity; ++j) {
                if (!blocks[i]->active[j]) {
                    blocks[i]->data[j] = value;
                    blocks[i]->active[j] = true;
                    ++blocks[i]->size;
                    ++total_size;
                    return iterator(this, blocks[i], j);
                }
            }
        }
    }
    allocate_new_block();
    return insert(value);
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::insert(value_type &&value) {
    for (size_type i = 0; i < block_count; ++i) {
        if (blocks[i]->size < block_capacity) {
            for (size_type j = 0; j < block_capacity; ++j) {
                if (!blocks[i]->active[j]) {
                    blocks[i]->data[j] = std::move(value);
                    blocks[i]->active[j] = true;
                    ++blocks[i]->size;
                    ++total_size;
                    return iterator(this, blocks[i], j);
                }
            }
        }
    }
    allocate_new_block();
    return insert(std::move(value));
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::erase(iterator it) {
    Block *block = it.block;
    size_type index = it.index;
    if (block && block->active[index]) {
        block->data[index].~T();
        block->active[index] = false;
        --block->size;
        --total_size;
//        std::cout << "Size: " << block->size << std::endl;
//        std::cout << "Capacity: " << block_count << std::endl;
        if (block->size == 0) {
            deallocate_block(block);
            //            std::cout << block << std::endl;
        }
        return ++it;
    }
    return end();
}

template<typename T>
bool BucketStorage<T>::empty() const noexcept {
    return total_size == 0;
}

template<typename T>
typename BucketStorage<T>::size_type BucketStorage<T>::size() const noexcept {
    return total_size;
}

template<typename T>
typename BucketStorage<T>::size_type BucketStorage<T>::capacity() const noexcept {
    return block_count * block_capacity;
}

template<typename T>
void BucketStorage<T>::shrink_to_fit() {
    for (size_type i = 0; i < block_count; ++i) {
        if (blocks[i]->size == 0) {
            deallocate_block(blocks[i]);
            for (size_type j = i; j < block_count - 1; ++j) {
                blocks[j] = blocks[j + 1];
            }
            --block_count;
            --i;
        }
    }
    Block **new_blocks = new Block *[block_count];
    for (size_type i = 0; i < block_count; ++i) {
        new_blocks[i] = blocks[i];
    }
    delete[] blocks;
    blocks = new_blocks;
}

template<typename T>
void BucketStorage<T>::clear() {
    if (blocks) {
        for (size_type i = 0; i < block_count; ++i) {
            deallocate_block(blocks[i]);
        }
        delete[] blocks;
        blocks = nullptr;
    }
    block_count = 0;
    total_size = 0;
}

template<typename T>
void BucketStorage<T>::swap(BucketStorage &other) noexcept {
    std::swap(block_capacity, other.block_capacity);
    std::swap(blocks, other.blocks);
    std::swap(block_count, other.block_count);
    std::swap(total_size, other.total_size);
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::begin() noexcept {
    for (size_type i = 0; i < block_count; ++i) {
        for (size_type j = 0; j < block_capacity; ++j) {
            if (blocks[i]->active[j]) {
                return iterator(this, blocks[i], j);
            }
        }
    }
    return end();
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::begin() const noexcept {
    for (size_type i = 0; i < block_count; ++i) {
        for (size_type j = 0; j < block_capacity; ++j) {
            if (blocks[i]->active[j]) {
                return const_iterator(this, blocks[i], j);
            }
        }
    }
    return end();
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::cbegin() const noexcept {
    return begin();
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::end() noexcept {
    return iterator(this, nullptr, 0);
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::end() const noexcept {
    return const_iterator(this, nullptr, 0);
}

template<typename T>
typename BucketStorage<T>::const_iterator BucketStorage<T>::cend() const noexcept {
    return end();
}

template<typename T>
typename BucketStorage<T>::iterator BucketStorage<T>::get_to_distance(iterator it, difference_type distance) {
    while (distance > 0) {
        ++it;
        --distance;
    }
    while (distance < 0) {
        --it;
        ++distance;
    }
    return it;
}

template<typename T>
void BucketStorage<T>::allocate_new_block() {
    Block *new_block = new Block(block_capacity);
    Block **new_blocks = new Block *[block_count + 1];
    for (size_type i = 0; i < block_count; ++i) {
        new_blocks[i] = blocks[i];
    }
    new_blocks[block_count] = new_block;
    delete[] blocks;
    blocks = new_blocks;
    ++block_count;
}

template<typename T>
void BucketStorage<T>::deallocate_block(Block *block) {
    if (block) {
        block->~Block();
    }
}
