////
//// Created by Alexander on 19.05.2024.
////
//
//#ifndef BUCKETSTORAGE_CONTAINER_H
//#define BUCKETSTORAGE_CONTAINER_H
//
//#include <iterator>
//#include <memory>
//#include <stdexcept>
//#include <vector>
//
//template<typename T>
//class BucketStorage {
//public:
//    using value_type = T;
//    using difference_type = std::ptrdiff_t;
//    using reference = value_type &;
//    using const_reference = const value_type &;
//    using pointer = value_type *;
//    using const_pointer = const value_type *;
//
//    class const_iterator;
//    class iterator;
//
//private:
//    struct Bucket {
//        std::unique_ptr<T[]> elements;
//        std::unique_ptr<bool[]> active;
//        size_t capacity;
//        size_t active_count;
//
//        explicit Bucket(size_t capacity)
//            : elements(std::make_unique<T[]>(capacity)),
//              active(std::make_unique<bool[]>(capacity)),
//              capacity(capacity),
//              active_count(0) {
//            std::fill(active.get(), active.get() + capacity, false);
//        }
//    };
//
//    std::vector<Bucket> buckets;
//    size_t bucket_capacity;
//    size_t total_size;
//
//public:
//    explicit BucketStorage(size_t bucket_capacity = 64)
//        : bucket_capacity(bucket_capacity), total_size(0) {}
//
//    BucketStorage(const BucketStorage &other)
//        : bucket_capacity(other.bucket_capacity), total_size(other.total_size) {
//        for (const auto &bucket: other.buckets) {
//            Bucket new_bucket(bucket.capacity);
//            std::copy(bucket.elements.get(), bucket.elements.get() + bucket.capacity, new_bucket.elements.get());
//            std::copy(bucket.active.get(), bucket.active.get() + bucket.capacity, new_bucket.active.get());
//            new_bucket.active_count = bucket.active_count;
//            buckets.push_back(std::move(new_bucket));
//        }
//    }
//
//    BucketStorage(BucketStorage &&other) noexcept
//        : buckets(std::move(other.buckets)), bucket_capacity(other.bucket_capacity), total_size(other.total_size) {
//        other.total_size = 0;
//    }
//
//    BucketStorage &operator=(BucketStorage other) {
//        swap(*this, other);
//        return *this;
//    }
//
//    friend void swap(BucketStorage &first, BucketStorage &second) noexcept {
//        using std::swap;
//        swap(first.buckets, second.buckets);
//        swap(first.bucket_capacity, second.bucket_capacity);
//        swap(first.total_size, second.total_size);
//    }
//
//    bool empty() const {
//        return total_size == 0;
//    }
//
//    size_t size_() const {
//        return total_size;
//    }
//
//    size_t capacity() const {
//        return buckets.size_() * bucket_capacity;
//    }
//
//    void shrink_to_fit() {
//        auto it = std::remove_if(buckets.begin(), buckets.end(),
//                                 [](const Bucket &bucket) { return bucket.active_count == 0; });
//        buckets.erase(it, buckets.end());
//    }
//
//    iterator insert(const T &value) {
//        return emplace(value);
//    }
//
//    iterator insert(T &&value) {
//        return emplace(std::move(value));
//    }
//
//    template<typename... Args>
//    iterator emplace(Args &&...args) {
//        for (auto &bucket: buckets) {
//            if (bucket.active_count < bucket.capacity) {
//                for (size_t i = 0; i < bucket.capacity; ++i) {
//                    if (!bucket.active[i]) {
//                        new (&bucket.elements[i]) T(std::forward<Args>(args)...);
//                        bucket.active[i] = true;
//                        ++bucket.active_count;
//                        ++total_size;
//                        return iterator(this, &bucket, i);
//                    }
//                }
//            }
//        }
//
//        buckets.emplace_back(bucket_capacity);
//        auto &new_bucket = buckets.back();
//        new (&new_bucket.elements[0]) T(std::forward<Args>(args)...);
//        new_bucket.active[0] = true;
//        ++new_bucket.active_count;
//        ++total_size;
//        return iterator(this, &new_bucket, 0);
//    }
//
//    iterator erase(const_iterator it) {
//        auto &bucket = *it.bucket;
//        size_t index = it.index;
//        if (bucket.active[index]) {
//            bucket.elements[index].~T();
//            bucket.active[index] = false;
//            --bucket.active_count;
//            --total_size;
//        }
//
//        auto next = it;
//        ++next;
//        return iterator(this, next.bucket, next.index);
//    }
//
//    iterator begin() {
//        return make_iterator_from_index(0);
//    }
//
//    const_iterator begin() const {
//        return make_iterator_from_index(0);
//    }
//
//    iterator end() {
//        return make_iterator_from_index(total_size);
//    }
//
//    const_iterator end() const {
//        return make_iterator_from_index(total_size);
//    }
//
//private:
//    iterator make_iterator_from_index(size_t index) {
//        size_t cumulative = 0;
//        for (auto &bucket: buckets) {
//            if (index < cumulative + bucket.active_count) {
//                for (size_t i = 0; i < bucket.capacity; ++i) {
//                    if (bucket.active[i]) {
//                        if (cumulative == index) {
//                            return iterator(this, &bucket, i);
//                        }
//                        ++cumulative;
//                    }
//                }
//            } else {
//                cumulative += bucket.active_count;
//            }
//        }
//        return end();
//    }
//
//public:
//    class const_iterator {
//        friend class BucketStorage;
//
//    protected:
//        const BucketStorage *storage;
//        const Bucket *bucket;
//        size_t index;
//
//        const_iterator(const BucketStorage *storage, const Bucket *bucket, size_t index)
//            : storage(storage), bucket(bucket), index(index) {}
//
//    public:
//        using iterator_category = std::forward_iterator_tag;
//        using value_type = BucketStorage::value_type;
//        using difference_type = BucketStorage::difference_type;
//        using pointer = BucketStorage::const_pointer;
//        using reference = BucketStorage::const_reference;
//
//        const_iterator() : storage(nullptr), bucket(nullptr), index(0) {}
//
//        reference operator*() const {
//            return bucket->elements[index];
//        }
//
//        pointer operator->() const {
//            return &bucket->elements[index];
//        }
//
//        const_iterator &operator++() {
//            do {
//                if (++index >= bucket->capacity) {
//                    auto it = std::find_if(storage->buckets.begin(), storage->buckets.end(),
//                                           [this](const Bucket &b) { return &b == bucket; });
//                    if (it != storage->buckets.end() && ++it != storage->buckets.end()) {
//                        bucket = &*it;
//                        index = 0;
//                    } else {
//                        bucket = nullptr;
//                        index = 0;
//                        break;
//                    }
//                }
//            } while (!bucket->active[index]);
//            return *this;
//        }
//
//        const_iterator operator++(int) {
//            const_iterator tmp = *this;
//            ++(*this);
//            return tmp;
//        }
//
//        bool operator==(const const_iterator &other) const {
//            return storage == other.storage && bucket == other.bucket && index == other.index;
//        }
//
//        bool operator!=(const const_iterator &other) const {
//            return !(*this == other);
//        }
//    };
//
//    class iterator : public const_iterator {
//        friend class BucketStorage;
//
//        iterator(BucketStorage *storage, Bucket *bucket, size_t index)
//            : const_iterator(storage, bucket, index) {}
//
//    public:
//        using pointer = BucketStorage::pointer;
//        using reference = BucketStorage::reference;
//
//        iterator() : const_iterator() {}
//
//        reference operator*() const {
//            return const_cast<reference>(const_iterator::operator*());
//        }
//
//        pointer operator->() const {
//            return const_cast<pointer>(const_iterator::operator->());
//        }
//
//        iterator &operator++() {
//            const_iterator::operator++();
//            return *this;
//        }
//
//        iterator operator++(int) {
//            iterator tmp = *this;
//            ++(*this);
//            return tmp;
//        }
//    };
//};
//
//#endif//BUCKETSTORAGE_CONTAINER_H
