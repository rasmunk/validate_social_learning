//
// Created by 40161642 on 07/07/2016.
//

#ifndef ROBOROBO_VECTOR_H
#define ROBOROBO_VECTOR_H
#include <vector>
template<class T>
class Vector {
protected:
    std::vector<T> _vector;
public:
    Vector() {};
    Vector(const Vector<T>& vector) { _vector = vector._vector; };
    Vector(const Vector<T>* vector) { _vector = vector->_vector; };
    virtual ~Vector() {};

    virtual T operator[](int i);
    virtual void add(T const&);
    virtual void setAt(int i, T const&);
    virtual bool contains(T const&);
    virtual bool containsCustomComparator(T const& data, std::function<bool(T, T)> const&);
    virtual int getIndexOf(T const&);
    virtual int getIndexOfCustomComparator(T const&, std::function<bool(T, T)> const&);
    virtual T getAt(int i);
    virtual void clear();
    virtual long long int size();
};

template<class T>
T Vector<T>::operator[](int i) {
    return _vector[i];
}

template<class T>
void Vector<T>::add(T const& data) {
    _vector.push_back(data);
}

template<class T>
void Vector<T>::setAt(int i, T const& data) {
    _vector[i] = data;
}


template<class T>
T Vector<T>::getAt(int i) {
    return _vector[i];
}

template<class T>
bool Vector<T>::contains(T const& data) {
    bool found = false;
    for (int i = 0; i<_vector.size(); ++i) {
        if (_vector[i] == data) {
            found = true;
        }
    }
    return found;
}

template<class T>
bool Vector<T>::containsCustomComparator(T const& data, std::function<bool(T, T)> const& compareFunction) {
    bool found = false;
    for (int i = 0; i<_vector.size(); ++i) {
        if (compareFunction(_vector[i], data)) {
            found = true;
        }
    }
    return found;
}

template<class T>
int Vector<T>::getIndexOf(T const& data) {
    int index = -1;
    for (int i = 0; i<_vector.size(); ++i) {
        if (_vector[i] == data) {
            index = i;
        }
    }
    return index;
}

template<class T>
int Vector<T>::getIndexOfCustomComparator(T const& data, std::function<bool(T, T)> const& compareFunction) {
    int index = -1;
    for (int i = 0; i<_vector.size(); ++i) {
        if (compareFunction(_vector[i],data)) {
            index = i;
        }
    }
    return index;
}

template<class T>
void Vector<T>::clear() {
    _vector.clear();
}

template<class T>
long long int Vector<T>::size() {
    return _vector.size();
}

#endif //ROBOROBO_VECTOR_H
