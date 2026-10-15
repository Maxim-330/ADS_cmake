// Copyright 2026 Onotin M
#ifndef MEMDATA_H
#define MEMDATA_H

#include <initializer_list>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <utility>

#define MEM_STEP 15

template <typename vector_type>
class Vector;

// --- Œ¡⁄ﬂ¬À≈Õ»≈  À¿——¿ ---
template <typename vector_type>
class MemData {
    vector_type* _data;
    size_t _size;
    size_t _capacity;

public:
    MemData(size_t size = 0);
    MemData(std::initializer_list<vector_type>);
    MemData(const vector_type*, size_t);
    MemData(const MemData&);
    MemData(MemData&&) noexcept;
    ~MemData();

    inline bool is_empty() const noexcept { return (_size == 0); }
    inline size_t get_size() const noexcept { return _size; }
    inline size_t get_capacity() const noexcept { return _capacity; }
    inline const vector_type* const get_data_const() const noexcept { return _data; }
    inline vector_type* const get_data_changeable() noexcept { return _data; }

    void set_memory(size_t) noexcept;
    void reset_memory(size_t size, size_t start_index = 0);
    void clear_memory() noexcept;

    inline void set_size(size_t size) {
        if (size > _capacity) {
            throw std::invalid_argument("ERROR: Size is bigger than capacity!");
        }
        else {
            _size = size;
        }
    }

    MemData& operator=(const MemData&) noexcept;
    MemData& operator=(MemData&&) noexcept;
    bool operator==(const MemData&) const noexcept;

    template <typename T>
    friend void quick_sort(MemData<T>& md);
    template <typename T>
    friend void shuffle(MemData<T>&);

    friend class Vector<vector_type>;

private:
    inline bool is_full() const noexcept { return (_size >= _capacity); }
};

// Œ·˙ˇ‚ÎÂÌËˇ ‚ÒÔÓÏÓ„‡ÚÂÎ¸Ì˚ı ÙÛÌÍˆËÈ
inline int calculate_capacity(size_t size);
template <typename vector_type>
void quick_sort_recursive(vector_type* data, int left, int right);
template <typename vector_type>
int partition(vector_type* data, int left, int right);

// --- –≈¿À»«¿÷»ﬂ ---

inline int calculate_capacity(size_t size) {
    size_t new_capacity = size / MEM_STEP;
    return (new_capacity + 1) * MEM_STEP;
}

template <typename vector_type>
MemData<vector_type>::MemData(size_t size) {
    _data = nullptr;
    set_memory(size);
    _size = 0;
}

template <typename vector_type>
MemData<vector_type>::MemData(std::initializer_list<vector_type> list) {
    _data = nullptr;
    set_memory(list.size());
    _size = list.size(); 
    size_t i = 0;
    for (auto& item : list) {
        _data[i++] = item;
    }
}

template <typename vector_type>
MemData<vector_type>::MemData(const vector_type* array, size_t size) {
    if (array == nullptr && size > 0) {
        throw std::invalid_argument("MemData: null array with non-zero size");
    }
    _size = size;
    _data = nullptr;
    set_memory(_size);
    for (size_t i = 0; i < _size; i++) {
        _data[i] = array[i];
    }
}

template <typename vector_type>
MemData<vector_type>::MemData(const MemData& other) {
    _size = other._size;
    _data = nullptr;
    set_memory(_size);
    for (size_t i = 0; i < _size; i++) {
        _data[i] = other._data[i];
    }
}

template <typename vector_type>
MemData<vector_type>::MemData(MemData&& other) noexcept {
    _size = other._size;
    _capacity = other._capacity;
    _data = other._data;
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
}

template <typename vector_type>
MemData<vector_type>::~MemData() {
    if (_data) {
        delete[] _data;
    }
}

template <typename vector_type>
void MemData<vector_type>::set_memory(size_t size) noexcept {
    _size = size;
    _capacity = calculate_capacity(size);
    if (_data) {
        delete[] _data;
    }
    _data = new vector_type[_capacity];
}

template <typename vector_type>
void MemData<vector_type>::reset_memory(size_t size, size_t start_index) {
    if (start_index >= size && size != 0) {
        throw std::invalid_argument("ERROR: Can't reset memory! start_index >= size");
    }
    size_t new_capacity = calculate_capacity(size);
    size_t old_capacity = _capacity;
    size_t copy_size = _size;

    if (new_capacity == old_capacity) {
        if (start_index == 0) {
            _size = size;
            return;
        }
        vector_type* temp = new vector_type[start_index];
        for (size_t i = 0; i < start_index; i++) {
            temp[i] = _data[i];
        }
        for (size_t i = 0; i < copy_size; i++) {
            if (i + start_index < old_capacity) {
                _data[i] = _data[(i + start_index)];
            }
            else {
                _data[i] = temp[(i + start_index) % old_capacity];
            }
        }
        _size = size;
        delete[] temp;
        return;
    }

    _capacity = new_capacity;
    vector_type* temp = new vector_type[_capacity];
    for (size_t i = 0; i < copy_size; i++) {
        if (i < size) {
            temp[i] = _data[(i + start_index) % old_capacity];
        }
    }

    delete[] _data;
    _data = temp;
    _size = size;
}


template <typename vector_type>
void MemData<vector_type>::clear_memory() noexcept {
    set_memory(0);
}

template <typename vector_type>
MemData<vector_type>& MemData<vector_type>::operator=(const MemData& other) noexcept {
    if (this != &other) {
        _size = other._size;
        _capacity = other._capacity;
        if (_data) {
            delete[] _data;
        }
        _data = new vector_type[_capacity];
        for (size_t i = 0; i < _size; i++) {
            _data[i] = other._data[i];
        }
    }
    return *this;
}

template <typename vector_type>
MemData<vector_type>& MemData<vector_type>::operator=(MemData&& other) noexcept {
    if (this != &other) {
        _size = other._size;
        _capacity = other._capacity;
        delete[] _data;
        _data = other._data;
    }
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
    return *this;
}

template <typename vector_type>
bool MemData<vector_type>::operator==(const MemData& other) const noexcept {
    if (_size != other._size) {
        return false;
    }
    else if (_data == other._data) {
        return true;
    }
    else if (_size == 0) {
        return true;
    }
    else {
        for (size_t i = 0; i < _size; i++) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }
        return true;
    }
}

template <typename vector_type>
void quick_sort(MemData<vector_type>& md) {
    if (md._size <= 1) {
        return;
    }
    quick_sort_recursive(md._data, 0, md._size - 1);
}

template <typename vector_type>
void shuffle(MemData<vector_type>& md) {
    if (md._size <= 1) {
        return;
    }
    srand(time(NULL));
    for (size_t i = md._size - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        std::swap(md._data[i], md._data[j]);
    }
}

template <typename vector_type>
int partition(vector_type* data, int left, int right) {
    vector_type pivot = data[(left + right) / 2];
    int i = left;
    int j = right;
    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    return i;
}

template <typename vector_type>
void quick_sort_recursive(vector_type* data, int left, int right) {
    if (left < right) {
        int pivot_index = partition(data, left, right);
        quick_sort_recursive(data, left, pivot_index - 1);
        quick_sort_recursive(data, pivot_index, right);
    }
}


#endif // MEMDATA_H