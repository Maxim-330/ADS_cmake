#pragma once
#include <cstddef>
#include <stdexcept>

template<class T>
class Queue {
    size_t _size;
    size_t _count;
    size_t _front;
    size_t _back;
    T* _data;

public:
    Queue(size_t size);
    Queue(const Queue& other);
    ~Queue();

    size_t size() const noexcept { return _count; }
    bool is_full() const noexcept { return _count == _size; }
    bool is_empty() const noexcept { return _count == 0; }

    Queue& operator=(const Queue&) = delete;
    T& front();
    const T& front() const;

    void push(const T& value);
    T pop();
};

template<class T>
Queue<T>::Queue(size_t size) : _size(size), _count(0), _front(0), _back(0),_data(nullptr)
{
    if (size == 0)
        throw std::invalid_argument("Queue capacity must be > 0");
    _data = new T[size];
}

template<class T>
Queue<T>::Queue(const Queue& other): _size(other._size), _count(other._count),_front(0), _back(other._count% other._size), _data(new T[other._size])
{
    for (size_t i = 0; i < other._count; ++i) _data[i] = other._data[(other._front + i) % other._size];
}

template<class T>
Queue<T>::~Queue() {
    delete[] _data;
}

template<class T>
void Queue<T>::push(const T& value) {
    if (is_full()) throw std::overflow_error("Queue is full");
    _data[_back] = value;
    _back = (_back + 1) % _size;
    ++_count;
}

template<class T>
T Queue<T>::pop() {
    if (is_empty()) throw std::underflow_error("Queue is empty");
    T tmp = _data[_front];
    _front = (_front + 1) % _size;
    --_count;
    return tmp;
}

template<class T>
T& Queue<T>::front() {
    if (is_empty()) throw std::underflow_error("Queue is empty");
    return _data[_front];
}

template<class T>
const T& Queue<T>::front() const {
    if (is_empty()) throw std::underflow_error("Queue is empty");
    return _data[_front];
}