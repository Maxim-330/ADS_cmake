// Copyright 2026 Onotin M
#include "memdata.h"

// --- ОБЪЯВЛЕНИЕ КЛАССА ---
template <typename vector_type>
class Vector {
protected:
    MemData<vector_type> _mem;
    size_t _front;
    size_t _back;

public:
    Vector(size_t size = 0);
    Vector(std::initializer_list<vector_type> list);
    Vector(vector_type* array, size_t size);
    Vector(const Vector<vector_type>& other);
    Vector(Vector<vector_type>&& other) noexcept;

    // Методы доступа
    inline bool is_empty() const noexcept { return _mem.is_empty(); }
    inline size_t get_size() const noexcept { return _mem.get_size(); }
    inline size_t get_capacity() const noexcept { return _mem.get_capacity(); }

    // Дружественные функции для тестов
    const MemData<vector_type>& get_mem_original() const { return _mem; }

    // Геттеры/Сеттеры элементов
    vector_type get_front() const;
    vector_type get_back() const;
    vector_type& front_ref();
    vector_type& back_ref();

    // Вставка
    void push_front(vector_type element) noexcept;
    void push_front_many(vector_type* elements, size_t size) noexcept;
    void push_back(vector_type element) noexcept;
    void push_back_many(vector_type* elements, size_t size) noexcept;
    void insert(vector_type element, size_t i);
    void insert_many(vector_type* elements, size_t size, size_t i);

    // Удаление
    void pop_front();
    void pop_front_many(size_t count);
    void pop_back();
    void pop_back_many(size_t count);
    void erase(size_t i);
    void erase_many(size_t i, size_t count);

    // Операторы
    Vector<vector_type>& operator=(const Vector<vector_type>& other) noexcept;
    Vector<vector_type>& operator=(Vector<vector_type>&& other) noexcept;
    vector_type& operator[](size_t i) noexcept;
    const vector_type& operator[](size_t i) const noexcept;

    // Вспомогательный метод для доступа к индексу в кольцевом буфере
    size_t get_mem_index(size_t i) const noexcept {
        return (_front + i) % _mem.get_capacity();
    }

    void shrink_to_fit();

    // Друзья
    template <typename T>
    friend void quick_sort(Vector<T>& vector);
    template <typename T>
    friend void shuffle(Vector<T>& vector);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& out, const Vector<T>& vector);
    template <typename T>
    friend std::istream& operator>>(std::istream& in, Vector<T>& vector);
};

//РЕАЛИЗАЦИЯ

template<typename vector_type>
Vector<vector_type>::Vector(size_t size) {
    MemData<vector_type> temp(size);
    _mem = (std::move(temp));
    _front = 0;
    if (size > 0) {
        _back = size - 1;
    }
    else {
        _back = 0;
    }
}

template<typename vector_type>
Vector<vector_type>::Vector(std::initializer_list<vector_type> list) {
    MemData<vector_type> temp(list);
    _mem = (std::move(temp));
    _front = 0;
    if (_mem.get_size() > 0) {
        _back = _mem.get_size() - 1;
    }
    else {
        _back = 0;
    }
}

template<typename vector_type>
Vector<vector_type>::Vector(vector_type* array, size_t size) {
    MemData<vector_type> temp(array, size);
    _mem = (std::move(temp));
    _front = 0;
    if (_mem.get_size() > 0) {
        _back = _mem.get_size() - 1;
    }
    else {
        _back = 0;
    }
}

template<typename vector_type>
Vector<vector_type>::Vector(const Vector<vector_type>& other) {
    _mem = other._mem;
    _front = other._front;
    _back = other._back;
}

template<typename vector_type>
Vector<vector_type>::Vector(Vector<vector_type>&& other) noexcept {
    _mem = std::move(other._mem);
    _front = other._front;
    other._front = 0;
    _back = other._back;
    other._back = 0;
}

template<typename vector_type>
vector_type Vector<vector_type>::get_front() const {
    if (is_empty()) throw std::logic_error("Vector is empty");
    return _mem.get_data_const()[_front];
}

template<typename vector_type>
vector_type Vector<vector_type>::get_back() const {
    if (is_empty()) throw std::logic_error("Vector is empty");
    return _mem.get_data_const()[_back];
}

template<typename vector_type>
vector_type& Vector<vector_type>::front_ref() {
    if (is_empty()) throw std::logic_error("Vector is empty");
    return _mem.get_data_changeable()[_front];
}

template<typename vector_type>
vector_type& Vector<vector_type>::back_ref() {
    if (is_empty()) throw std::logic_error("Vector is empty");
    return _mem.get_data_changeable()[_back];
}

template<typename vector_type>
void Vector<vector_type>::push_front(vector_type element) noexcept {
    size_t current_size = _mem.get_size();
    _mem.set_size(current_size + 1);

    if (_mem.get_size() > 1) {
        _front = (_front + _mem.get_capacity() - 1) % _mem.get_capacity();
    }
    _mem.get_data_changeable()[_front] = element;

    if (_mem.is_full()) {
        _mem.reset_memory(_mem.get_size(), _front);
        _front = 0;
        _back = _mem.get_size() - 1;
    }
}

template<typename vector_type>
void Vector<vector_type>::push_front_many(vector_type* elements, size_t size) noexcept {
    for (int i = size - 1; i >= 0; i--) {
        push_front(elements[i]);
    }
}

template<typename vector_type>
void Vector<vector_type>::push_back(vector_type element) noexcept {
    size_t current_size = _mem.get_size();
    _mem.set_size(current_size + 1);

    if (_mem.get_size() > 1) {
        _back = (_back + 1) % _mem.get_capacity();
    }
    _mem.get_data_changeable()[_back] = element;

    if (_mem.is_full()) {
        _mem.reset_memory(_mem.get_size(), _front);
        _front = 0;
        _back = _mem.get_size() - 1;
    }
}

template<typename vector_type>
void Vector<vector_type>::push_back_many(vector_type* elements, size_t size) noexcept {
    for (size_t i = 0; i < size; i++) {
        push_back(elements[i]);
    }
}

template<typename vector_type>
void Vector<vector_type>::insert(vector_type element, size_t i) {
    if (i > _mem.get_size()) {
        throw std::out_of_range("ERROR: Insert index out of range!");
    }
    else {
        if (i == 0) {
            push_front(element);
        }
        else if (i == _mem.get_size()) {
            push_back(element);
        }
        else {
            _mem.set_size(_mem.get_size() + 1);
            _back = (_back + 1) % _mem.get_capacity();
            for (int j = _mem.get_size() - 1; j > i; j--) {
                (*this)[j] = (*this)[j - 1];
            }
            (*this)[i] = element;
            if (_mem.is_full()) {
                _mem.reset_memory(_mem.get_size(), _front);
                _front = 0;
                _back = _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::insert_many(vector_type* elements, size_t size, size_t i) {
    if (i > _mem.get_size()) {
        throw std::out_of_range("ERROR: Insert (many) index out of range!");
    }
    else {
        for (size_t j = 0; j < size; j++) {
            insert(elements[j], i + j);
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::pop_front() {
    if (_mem.get_size() == 0) {
        throw std::logic_error("ERROR: Empty vector! Can't pop front");
    }
    else {
        _mem.set_size(_mem.get_size() - 1);
        if (_mem.get_size() != 0) {
            _front = (_front + 1) % _mem.get_capacity();
        }
        else {
            _front = 0;
            _back = 0;
        }
        if (_mem.get_capacity() - _mem.get_size() > MEM_STEP) {
            _mem.reset_memory(_mem.get_size(), _front);
            _front = 0;
            if (_mem.get_size() != 0) {
                _back = _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::pop_front_many(size_t count) {
    if (_mem.get_size() < count) {
        throw std::logic_error("ERROR: Popping (front) too many elements!");
    }
    else {
        _mem.set_size(_mem.get_size() - count);
        if (_mem.get_size() != 0) {
            _front = (_front + count) % _mem.get_capacity();
        }
        else {
            _front = 0;
            _back = 0;
        }
        if (_mem.get_capacity() - _mem.get_size() > MEM_STEP) {
            _mem.reset_memory(_mem.get_size(), _front);
            _front = 0;
            if (_mem.get_size() != 0) {
                _back = _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::pop_back() {
    if (_mem.get_size() == 0) {
        throw std::logic_error("ERROR: Empty vector! Can't pop back");
    }
    else {
        _mem.set_size(_mem.get_size() - 1);
        if (_mem.get_size() != 0) {
            _back = (_back + _mem.get_capacity() - 1) % _mem.get_capacity();
        }
        else {
            _front = 0;
            _back = 0;
        }
        if (_mem.get_capacity() - _mem.get_size() > MEM_STEP) {
            _mem.reset_memory(_mem.get_size(), _front);
            _front = 0;
            if (_mem.get_size() != 0) {
                _back = _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::pop_back_many(size_t count) {
    if (_mem.get_size() < count) {
        throw std::logic_error("ERROR: Popping (back) too many elements!");
    }
    else {
        _mem.set_size(_mem.get_size() - count);
        if (_mem.get_size() != 0) {
            _back = (_back + _mem.get_capacity() - count) % _mem.get_capacity();
        }
        else {
            _front = 0;
            _back = 0;
        }
        if (_mem.get_capacity() - _mem.get_size() > MEM_STEP) {
            _mem.reset_memory(_mem.get_size(), _front);
            _front = 0;
            if (_mem.get_size() != 0) {
                _back = _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::erase(size_t i) {
    if (i >= _mem.get_size()) {
        throw std::out_of_range("ERROR: Erase index out of range!");
    }
    else {
        if (i == 0) {
            pop_front();
        }
        else if (i == _mem.get_size() - 1) {
            pop_back();
        }
        else {
            _back = (_back - 1 + _mem.get_capacity()) % _mem.get_capacity();
            for (size_t j = i; j < _mem.get_size() - 1; j++) {
                (*this)[j] = (*this)[j + 1];
            }
            _mem.set_size(_mem.get_size() - 1);
            if (_mem.get_size() == 0) {
                _front = 0;
                _back = 0;
            }
            if (_mem.get_capacity() - _mem.get_size() > MEM_STEP) {
                _mem.reset_memory(_mem.get_size(), _front);
                _front = 0;
                _back = _mem.get_size() == 0 ? 0 : _mem.get_size() - 1;
            }
        }
    }
}

template<typename vector_type>
void Vector<vector_type>::erase_many(size_t i, size_t count) {
    if (i + count > _mem.get_size()) {
        throw std::logic_error("ERROR: Erasing too many elements!");
    }
    for (size_t j = 0; j < count; j++) {
        erase(i);
    }
}

template<typename vector_type>
Vector<vector_type>& Vector<vector_type>::operator=(const Vector<vector_type>& other) noexcept {
    if (this != &other) {
        _mem = other._mem;
        _front = other._front;
        _back = other._back;
    }
    return (*this);
}

template<typename vector_type>
Vector<vector_type>& Vector<vector_type>::operator=(Vector<vector_type>&& other) noexcept {
    if (this != &other) {
        _mem = std::move(other._mem);
        _front = other._front;
        other._front = 0;
        _back = other._back;
        other._back = 0;
    }
    return (*this);
}

template<typename vector_type>
vector_type& Vector<vector_type>::operator[](size_t i) noexcept {
    return _mem.get_data_changeable()[get_mem_index(i)];
}

template<typename vector_type>
const vector_type& Vector<vector_type>::operator[](size_t i) const noexcept {
    return _mem.get_data_const()[get_mem_index(i)];
}

template<typename vector_type>
void quick_sort(Vector<vector_type>& vector) {
    quick_sort(vector._mem);
}

template<typename vector_type>
void shuffle(Vector<vector_type>& vector) {
    shuffle(vector._mem);
}

template <typename vector_type>
std::ostream& operator<< (std::ostream& out, const Vector<vector_type>& vector) {
    out << "{ ";
    size_t size = vector.get_size();
    if (size != 0) {
        out << vector._mem.get_data_const()[vector._front];
    }
    for (size_t i = 1; i < size; i++) {
        out << ", " << vector._mem.get_data_const()[vector.get_mem_index(i)];
    }
    out << " }";
    return out;
}

template <typename vector_type>
std::istream& operator>> (std::istream& in, Vector<vector_type>& vector) {
    size_t size;
    if (in >> size) {
        Vector<vector_type> temp;
        vector_type element;
        for (size_t i = 0; i < size; i++) {
            if (in >> element) {
                temp.push_back(element);
            }
            else {
                break;
            }
        }
        vector = std::move(temp);
    }
    return in;
}
template<typename vector_type>
void Vector<vector_type>::shrink_to_fit() {
    //Удаляем нули с конца
    while (_mem.get_size() > 0 && (*this)[_mem.get_size() - 1] == vector_type(0)) {
        pop_back();
    }

    //Если вместимость больше чем нужно перевыделяем память
    if (_mem.get_capacity() > _mem.get_size()) {
        _mem.reset_memory(_mem.get_size(), _front);
        _front = 0;
        if (_mem.get_size() > 0) {
            _back = _mem.get_size() - 1;
        }
        else {
            _back = 0;
        }
    }
}
template class Vector<double>;
template std::ostream& operator<< (std::ostream& out, const Vector<double>& v1);
template std::istream& operator>> (std::istream& in, Vector<double>& v1);
template void shuffle(Vector<double>&);
template void quick_sort(Vector<double>&);