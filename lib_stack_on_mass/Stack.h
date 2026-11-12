// Copyright 2026 Maksim Onotin

#ifndef LIB_STACK_ON_MASS_STACK_H_
#define LIB_STACK_ON_MASS_STACK_H_

#include <cstddef>
#include <stdexcept>

template <class T>
class StackOnMass {
  T* _data;
  size_t _size;
  size_t _top;

 public:
  explicit StackOnMass(size_t size);
  StackOnMass(const StackOnMass& other);
  ~StackOnMass();

  void push(const T& val);
  T pop();
  inline T& top();
  inline bool is_empty() const noexcept;
  inline bool is_full() const noexcept;
};

template <class T>
StackOnMass<T>::StackOnMass(size_t size) : _size(size), _top(0) {
  if (size == 0) {
    throw std::invalid_argument("Stack size must be greater than 0");
  }
  _data = new T[size];
}

template <class T>
StackOnMass<T>::StackOnMass(const StackOnMass& other)
    : _size(other._size), _top(other._top) {
  _data = new T[_size];
  for (size_t i = 0; i < _top; ++i) {
    _data[i] = other._data[i];
  }
}

template <class T>
StackOnMass<T>::~StackOnMass() {
  delete[] _data;
}

template <class T>
void StackOnMass<T>::push(const T& val) {
  if (is_full()) {
    throw std::overflow_error("Stack overflow: cannot push to full stack");
  }
  _data[_top++] = val;
}

template <class T>
T StackOnMass<T>::pop() {
  if (is_empty()) {
    throw std::underflow_error("Stack underflow: cannot pop from empty stack");
  }
  return _data[--_top];
}

template <class T>
inline T& StackOnMass<T>::top() {
  if (is_empty()) {
    throw std::underflow_error("Stack is empty: no top element");
  }
  return _data[_top - 1];
}

template <class T>
inline bool StackOnMass<T>::is_empty() const noexcept {
  return _top == 0;
}

template <class T>
inline bool StackOnMass<T>::is_full() const noexcept {
  return _top == _size;
}

#endif  // LIB_STACK_ON_MASS_STACK_H_
