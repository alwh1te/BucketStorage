#include "iostream"
#include "stdexcept"


template<typename T>
class Vector {
private:
    unsigned int currCapacity;
    unsigned int vectSize;

    T *buffer;

public:
    typedef T *iterator;
    Vector();
    Vector(unsigned int size);
    Vector(unsigned int size, const T &type);
    Vector(const Vector<T> &other);    //Copy Constructor
    Vector(Vector<T> &&other) noexcept;//Move Constructor
    ~Vector();
    Vector<T> &operator=(const Vector<T> &other);
    Vector<T> &operator=(Vector<T> &&other) noexcept;

    iterator begin();
    iterator end();
    /*iterator rbegin();
    iterator rend();
    iterator cbegin() const;
    iterator cend() const;
    iterator crbegin() const;
    iterator crend() const;*/


    unsigned int size() const;
    unsigned int max_size() const;
    unsigned int capacity() const;
    bool empty() const;
    void reserve(unsigned int cap);
    void shrink_to_fit();

    T &operator[](unsigned int idx);
    T &operator[](unsigned int idx) const;
    T &at(unsigned int idx);
    T &at(unsigned int idx) const;
    T &front() const;
    T &back() const;
    T *data() const;

    void assign(unsigned int count, const T &value);
    void push_back(const T &item);
    void push_front(const T &item);
    void pop_back();
    void insert(iterator it, const T &item);
    void insert(unsigned int pos, const T &item);
    void erase(iterator it);
    void erase(iterator first, iterator last);
    void resize(unsigned int size);
    void swap(Vector<T> &otherV) noexcept;
    void clear();

    // TODO Delete
    template<class... Args>
    void emplace(iterator it, Args &&...args) {
        //T newT(std::forward<Args>(args)...));
        T *newT = new (it) T(std::forward<Args>(args)...);
        //push_back(new (*this) T(std::forward<Args>(args)...));
    }
    template<class... Args>
    void emplace_back(Args &&...args) {
        //T newT(std::forward<Args>(args)...));
        if (vectSize == currCapacity) {
            if (vectSize == 0) reserve(1);
            else
                reserve(2 * currCapacity);
        }
        T *newT = new (end()) T(std::forward<Args>(args)...);
        //push_back(new (*this) T(std::forward<Args>(args)...));
        ++vectSize;
    }
    //void get_allocator
};


/** Constructors and Destructors **/
template<typename T>
Vector<T>::Vector() {
    //cout << "In Constructor #1"<<"\n";
    currCapacity = 0;
    vectSize = 0;
    buffer = nullptr;
}

template<typename T>
Vector<T>::Vector(unsigned int size) {
    //cout << "In Constructor #2"<<"\n";
    buffer = nullptr;
    this->vectSize = size;
    reserve(size);
}

template<typename T>
Vector<T>::Vector(unsigned int size, const T &item) {

    //cout << "In Constructor #3"<<"\n";
    if (this->buffer != nullptr) this->buffer = nullptr;
    reserve(size);
    this->vectSize = size;
    for (unsigned int i = 0; i < size; i++) memcpy(buffer + i, &item, sizeof(item));
}

template<typename T>
Vector<T>::Vector(const Vector<T> &other) {
    //cout << "In Copy Constructor #4"<<"\n";
    this->buffer = nullptr;
    reserve(other.size());
    vectSize = other.size();
    for (unsigned int i = 0; i < other.size(); i++) buffer[i] = other.buffer[i];
}

template<typename T>
Vector<T>::Vector(Vector<T> &&other) noexcept {
    //cout << "In Move Constructor #5"<<"\n";
    buffer = other->buffer;
    other->buffer = nullptr;
}

template<typename T>
Vector<T>::~Vector() {
    //cout << "In Custom Vector Destructor "<<"\n";
    if (buffer != nullptr) {
        delete[] buffer;
        buffer = nullptr;
    }
}

template<typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    //cout << "In operator= (...)"<<"\n";
    Vector<T> tmpCpy(other);
    tmpCpy.swap(*this);
    return *this;
}
template<typename T>
Vector<T> &Vector<T>::operator=(Vector<T> &&other) noexcept {
}

/** Iterators **/
template<typename T>
typename Vector<T>::iterator Vector<T>::begin() {
    return buffer;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end() {
    return buffer + size();
}


/** Capacity Functions **/
template<typename T>
unsigned int Vector<T>::size() const {
    //cout << "In size()"<<"\n";
    return vectSize;
}

template<typename T>
unsigned int Vector<T>::max_size() const {
    //cout << "In max_size()"<<"\n";
    return UINT_MAX;
}

template<typename T>
bool Vector<T>::empty() const {
    //cout << "In empty()"<<"\n";
    return size() == 0;
}


template<typename T>
void Vector<T>::reserve(unsigned int cap) {
    //cout << "In reserve(..)"<<"\n";
    T *newArr;
    try {
        newArr = new T[(sizeof(T) * cap)];
    } catch (const std::bad_alloc &ba) {
        std::cerr << "Vector reserve(): memory allocation failure: " << ba.what() << "\n";
    }

    if (buffer != nullptr) {
        for (unsigned int i = 0; i < vectSize; i++) newArr[i] = buffer[i];
        delete[] buffer;
        buffer = nullptr;
    }
    buffer = newArr;
    currCapacity = cap;
}

template<typename T>
unsigned int Vector<T>::capacity() const {
    //cout << "In capacity()"<<"\n";
    return currCapacity;
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    //cout << "In shrink_to_fit()"<<"\n";
    for (auto it = end(); it < begin() + currCapacity; it++) it->~T();
    currCapacity = vectSize;
}


/** Access **/
template<typename T>
T &Vector<T>::operator[](unsigned int idx) {
    //cout << "In operator[] (...)"<<"\n";
    return buffer[idx];
}
template<typename T>
T &Vector<T>::operator[](unsigned int idx) const {
    //cout << "In operator[] (...)"<<"\n";
    return buffer[idx];
}

template<typename T>
T &Vector<T>::at(unsigned int idx) {
    //cout << "In at(...)"<<"\n";
    try {
        return buffer[idx];
    } catch (const std::out_of_range &oor) {
        std::cerr << "Out of Range error: " << oor.what() << "\n";
    }
}

template<typename T>
T &Vector<T>::at(unsigned int idx) const {
    //cout << "In at(...)"<<"\n";
    try {
        return buffer[idx];
    } catch (const std::out_of_range &oor) {
        std::cerr << "Out of Range error: " << oor.what() << "\n";
    }
}

template<typename T>
T &Vector<T>::front() const {
    //cout << "In front()"<<"\n";
    return buffer[0];
}

template<typename T>
T &Vector<T>::back() const {
    //cout << "In back()"<<"\n";
    return buffer[vectSize - 1];
}

template<typename T>
T *Vector<T>::data() const {
    //cout << "In data()"<<"\n";
    return buffer;
}


/** Modifiers **/
template<typename T>
void Vector<T>::clear() {
    //cout << "In clear()"<<"\n";
    if (buffer != nullptr) delete[] buffer;
    buffer = nullptr;
    vectSize = 0;
}

template<typename T>
void Vector<T>::assign(unsigned int count, const T &item) {
    //cout << "In assign(...)"<<"\n";
    resize(count);
    for (unsigned int i = 0; i < count; i++) this->at(i) = item;
}

template<typename T>
void Vector<T>::push_back(const T &item) {
    //cout << "In push_back(...)"<<"\n";
    if (vectSize == currCapacity) {

        if (currCapacity == 0) reserve(1);
        else
            reserve(2 * currCapacity);
    }
    buffer[vectSize++] = item;
}


template<typename T>
void Vector<T>::push_front(const T &item) {
    //cout << "In push_front(...)"<<"\n";
    if (vectSize == currCapacity) {
        if (currCapacity == 0) reserve(1);
        else
            reserve(2 * currCapacity);
    }
    for (unsigned int i = vectSize; i >= 1; i--) buffer[i] = buffer[i - 1];
    buffer[0] = item;
    ++vectSize;
}

template<typename T>
void Vector<T>::pop_back() {
    //cout << "In pop_back()"<<"\n";
    --vectSize;
    (end() - 1)->~T();
}

template<typename T>
void Vector<T>::insert(Vector<T>::iterator it, const T &item) {
    //cout << "In insert(...)"<<"\n";
    *it = item;
}
template<typename T>
void Vector<T>::insert(unsigned int pos, const T &item) {
    //cout << "In insert(...)"<<"\n";
    this->buffer[pos] = item;
}

template<typename T>
void Vector<T>::erase(Vector<T>::iterator it) {
    //cout << "In erase(...) #1"<<"\n";
    if (it == end() - 1) {
        pop_back();
    } else {
        it->~T();
        for (auto i = it; i < end(); i++) {
            if (i + 1 < end()) *i = *(i + 1);
        }
        --vectSize;
    }
}

template<typename T>
void Vector<T>::erase(Vector<T>::iterator first, Vector<T>::iterator last) {
    //cout << "In erase(...) #2"<<"\n";
    int count = 0;
    for (auto it = first; it < last; it++) {
        ++count;
        it->~T();
    }
    auto it = first;
    auto it2 = last;
    while (it2 < end()) {
        *it++ = *it2++;
        it2->~T();
    }
    vectSize = vectSize - count;
}

template<typename T>
void Vector<T>::resize(unsigned int size) {
    //cout << "In resize(...)"<<"\n";
    T *newArr = new T[size];

    int stopIdx = (size > vectSize) ? vectSize : size;

    for (unsigned int i = 0; i < stopIdx; i++) newArr[i] = buffer[i];
    if (buffer != nullptr) delete[] buffer;
    buffer = newArr;
    currCapacity = size;
    vectSize = size;
}

template<typename T>
void Vector<T>::swap(Vector<T> &otherV) noexcept {
    //cout << "In swap(...)"<<"\n";
    using std::swap;
    unsigned int i = otherV.size();
    swap(this->vectSize, i);
    swap(this->buffer, otherV.buffer);
}
