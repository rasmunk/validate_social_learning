//
// Created by 40161642 on 08/06/2016.
//

#ifndef ROBOROBO_STACK_H
#define ROBOROBO_STACK_H

#include <stack>

template <class T>
class Stack {
protected:
    std::stack<T> _stack;

public:
    Stack() {};
    Stack(const Stack<T>& stack) { _stack = stack._stack; };
    Stack(const Stack<T>* stack) { _stack = stack->_stack; };
    virtual ~Stack();
    virtual void push(const T& object);
    virtual T pop();
    virtual long long int size();
    virtual void clear();
    bool Empty();
};

template<class T>
Stack<T>::~Stack() {
    this->clear();
}

template<class T>
void Stack<T>::push(const T &object) {
    _stack.push(object);
}

template<class T>
T Stack<T>::pop() {
    T data;
    if (!this->Empty()) {
        data = _stack.top();
        _stack.pop();
    }
    return data;
}

template<class T>
bool Stack<T>::Empty() {
    return _stack.empty();
}

template<class T>
long long int Stack<T>::size() {
    return _stack.size();
}

template<class T>
void Stack<T>::clear() {
    while (!this->Empty()) {
        this->pop();
    }
}

#endif //ROBOROBO_STACK_H
