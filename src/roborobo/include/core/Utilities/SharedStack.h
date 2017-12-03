//
// Created by 40161642 on 29/06/2016.
//

#ifndef ROBOROBO_SHAREDSTACK_H
#define ROBOROBO_SHAREDSTACK_H
#include "core/Utilities/SharedDataStructure.h"
#include "core/Utilities/Stack.h"
#include <mutex>

template<class T>
class SharedStack : public SharedDataStructure, public Stack<T> {
private:
    std::mutex _stackLock;

public:
    SharedStack() : Stack<T>() {};
    SharedStack(const SharedStack<T>& sharedStack);
    ~SharedStack() {};
    virtual SharedStack<T> clone();
    void push(const T &object);
    void pushSizeConstraint(const T &object, int size);
    long long int size();
    T pop();
};

template<class T>
SharedStack<T>::SharedStack(const SharedStack<T> &sharedStack) {
    this->_stack = sharedStack._stack;
}

template<class T>
SharedStack<T> SharedStack<T>::clone() {
    std::lock_guard<std::mutex> lock(_stackLock);
    SharedStack<T> copy = SharedStack<T>(*this);
    return copy;
}

template<class T>
void SharedStack<T>::push(const T &object) {
    std::lock_guard<std::mutex> lock(_stackLock);
    Stack<T>::push(object);
}

template<class T>
void SharedStack<T>::pushSizeConstraint(const T &object, int size) {
    std::lock_guard<std::mutex> lock(_stackLock);
    if (Stack<T>::size() < size) {
        Stack<T>::push(object);
    }
}

template<class T>
T SharedStack<T>::pop() {
    T data;
    _stackLock.lock();
    data = Stack<T>::pop();
    _stackLock.unlock();
    return data;
}

template<class T>
long long int SharedStack<T>::size() {
    return Stack<T>::size();
}

#endif //ROBOROBO_SHAREDSTACK_H
