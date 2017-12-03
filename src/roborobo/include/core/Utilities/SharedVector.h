//
// Created by 40161642 on 29/06/2016.
//

#ifndef ROBOROBO_SHAREDVECTOR_H
#define ROBOROBO_SHAREDVECTOR_H

#include "core/Utilities/SharedDataStructure.h"
#include "core/Utilities/Vector.h"
#include <mutex>
template<class T>
class SharedVector : public SharedDataStructure, public Vector<T> {
private:
    std::mutex _mutex;

public:
    SharedVector() : Vector<T>() {};
    SharedVector(const SharedVector<T>& sharedVector);
    ~SharedVector() {};
    virtual SharedVector<T> clone();
    void add(T const&);
    bool contains(T const&);
    void addOrReplaceUnique(T const&);
    void addOrReplaceUniqueCustomComparator(T const& data, std::function<bool(T, T)> const&);
    void addOrReplaceUniqueCustomComparatorSizeConstraint(T const& data, std::function<bool(T, T)> const&, int size);
    T getAt(int i);
    void clear();
    long long int size();
};

template<class T>
SharedVector<T>::SharedVector(const SharedVector<T>& sharedVector) {
    this->_vector = sharedVector._vector;
}

template<class T>
SharedVector<T> SharedVector<T>::clone() {
    std::lock_guard<std::mutex> lock(_mutex);
    SharedVector<T> copy = SharedVector<T>(*this);
    return copy;
}

template<class T>
void SharedVector<T>::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    Vector<T>::clear();
}

template<class T>
void SharedVector<T>::add(T const& data) {
    std::lock_guard<std::mutex> lock(_mutex);
    Vector<T>::add(data);
}


template<class T>
bool SharedVector<T>::contains(T const& data) {
    bool found = false;
    _mutex.lock();
    found = Vector<T>::contains(data);
    _mutex.unlock();
    return found;
}

template<class T>
void SharedVector<T>::addOrReplaceUnique(T const& data) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (Vector<T>::contains(data)) {
        Vector<T>::setAt(Vector<T>::getIndexOf(data), data);
    } else {
        Vector<T>::add(data);
    }
}

template<class T>
void SharedVector<T>::addOrReplaceUniqueCustomComparator(T const& data, std::function<bool(T, T)> const& compareFunction) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (Vector<T>::containsCustomComparator(data, compareFunction)) {
        Vector<T>::setAt(Vector<T>::getIndexOfCustomComparator(data, compareFunction), data);
    } else {
        Vector<T>::add(data);
    }
}

template<class T>
void SharedVector<T>::addOrReplaceUniqueCustomComparatorSizeConstraint(T const& data, std::function<bool(T, T)> const& compareFunction, int size) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (Vector<T>::containsCustomComparator(data, compareFunction)) {
        Vector<T>::setAt(Vector<T>::getIndexOfCustomComparator(data, compareFunction), data);
    } else {
        if (Vector<T>::size() < size) {
            Vector<T>::add(data);
        }
    }
}

template<class T>
T SharedVector<T>::getAt(int i) {
    return Vector<T>::getAt(i);
}

template<class T>
long long int SharedVector<T>::size() {
    return Vector<T>::size();
}

#endif //ROBOROBO_SHAREDVECTOR_H
