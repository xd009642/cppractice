#ifndef XD_VECTOR_H
#define XD_VECTOR_H
#include <stdexcept>
#include <cstring>
#include <iterator>
#include <initializer_list>
#include <limits>


namespace xd {

    template<typename T> 
    class vector {
    public:
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;
        using pointer = T*;
        using iterator = pointer;
        using const_pointer = const T*;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;

        // default constructor
        vector();
        // Fill with a given value
        vector(size_t count, const_reference value);
        vector(size_t count);
        vector(const_iterator first, const_iterator last);
        vector(std::initializer_list<T> l);

        ~vector();

        vector& operator=(std::initializer_list<T> il);
        
        void assign(size_t count, const_reference value);
        void assign(const_iterator first, const_iterator last);
        void assign(std::initializer_list<T> il);

        void reserve(size_t capacity);

        void push_back(const_reference value);
        void push_back(const T&& value);

        template<typename... Args>
        reference emplace_back(Args&&... args);
        
        template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, const T&& value);
        iterator insert(const_iterator pos, size_t count, const T& value);
        iterator insert(const_iterator pos, const_iterator first, const_iterator last);
        iterator insert(const_iterator pos, std::initializer_list<T> il);

        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        void pop_back();

        void resize(size_t count);
        void resize(size_t count, const T& value);

        void clear();

        reference operator[](size_t index);
        const_reference operator[](size_t index) const;

        reference at(size_t index);
        const_reference at(size_t index) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        size_t size() const noexcept;
        size_t capacity() const noexcept;
        bool empty() const noexcept;
        size_t max_size() const noexcept;

        void shrink_to_fit();

        pointer data() noexcept;

        const_pointer data() const noexcept;

        void swap(vector<T>& other) noexcept;

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;
        
        iterator rbegin() noexcept;
        const_iterator rbegin() const noexcept;
        const_iterator crbegin() const noexcept;

        iterator rend() noexcept;
        const_iterator rend() const noexcept;
        const_iterator crend() const noexcept;
    protected:
        size_t next_capacity() const {
            return _capacity==0?1:_capacity*2;
        }
    private:
        //! Current size of the vector
        size_t raw_size;
        //! Current capacity 
        //!(capacity >= size to prevent continuously allocating)
        size_t _capacity;
        //! Array containing the data
        T* _data;
    };



    template<typename T>
    vector<T>::vector():
    raw_size(0),
    _capacity(0),
    _data(nullptr) {
    }

    template<typename T>
    vector<T>::vector(size_t count, const T& value):vector() {
        assign(count, value);
    }

    template<typename T>
    vector<T>::vector(size_t count):vector(count, T()) {
    }

    template<typename T>
    vector<T>::vector(vector<T>::const_iterator begin, vector<T>::const_iterator end):vector(){
        assign(begin, end);
    }

    template<typename T>
    vector<T>::vector(std::initializer_list<T> il):vector(il.begin(), il.end()){
    }

    template<typename T>
    vector<T>::~vector() {
        clear();
    }

    template<typename T>
    void vector<T>::assign(size_t count, const T& value) {
        clear();
        reserve(count);
        for(size_t i=0; i<count; i++) {
            push_back(value);
        }
    }

    template<typename T>
    void vector<T>::assign(vector<T>::const_iterator first, vector<T>::const_iterator last) {
        clear();
        reserve(std::distance(first, last));
        for(auto it=first; it!=last; it++) {
            push_back(*it);
        }
    }

    template<typename T> 
    void vector<T>::assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    template<typename T>
    vector<T>& vector<T>::operator=(std::initializer_list<T> il) {
        clear();
        reserve(std::distance(il.begin(), il.end()));
        for(auto it=il.begin(); it!=il.end(); it++) {
            push_back(*it);
        }
        return *this;
    }

    template<typename T>
    void vector<T>::reserve(size_t cap) {
        if(cap <= _capacity) {
            return;
        }
        T* new_data = new T[cap];
        if(_data != nullptr) {
            memcpy(new_data, _data, raw_size*sizeof(T));
            delete[] _data;
        }
        _data = new_data;
        _capacity = cap;
    }

    template<typename T>
    template<typename... Args>
    T& vector<T>::emplace_back(Args&&... args) {
        if((raw_size + 1) > _capacity) {
            // Double rate growth factor just to be simple
            reserve(next_capacity());
        }
        _data[raw_size] = std::move(T(std::forward<Args>(args)...));
        raw_size++;
        return _data[raw_size-1];
    }
    
    template<typename T>
    template<typename... Args>
    T* vector<T>::emplace(const T* pos, Args&&... args) {
        insert(pos, T(std::forward<Args>(args)...));
    }

    template<typename T> 
    void vector<T>::push_back(const T& value) {
        if((raw_size + 1) > _capacity) {
            reserve(next_capacity());
        }
        _data[raw_size] = value;
        raw_size++;
    }

    template<typename T> 
    void vector<T>::push_back(const T&& value) {
        if((raw_size + 1) > _capacity) {
            reserve(next_capacity());
        }
        _data[raw_size] = std::move(value);
        raw_size++;
    }

    template<typename T>
    T* vector<T>::insert(const T* pos, const T& value) {
        if(pos == end()) {
            push_back(value);
            return rend();
        }
        // If we work out index after reserve pointer distance is wrong!
        size_t index = std::distance(cbegin(), pos);
        if((raw_size + 1) > _capacity) {
            reserve(next_capacity());
        }
        memmove(_data+index+1, _data+index, (raw_size-index)*sizeof(T));
        _data[index] = value;
        
        raw_size++;
        return _data+index;
    }
    
    template<typename T>
    T* vector<T>::insert(const T* pos, const T&& value) {
        if(pos == end()) {
            push_back(value);
            return rend();
        }
        // If we work out index after reserve pointer distance is wrong!
        size_t index = std::distance(cbegin(), pos);
        if((raw_size + 1) > _capacity) {
            reserve(next_capacity());
        }
        memmove(_data+index+1, _data+index, (raw_size-index)*sizeof(T));
        _data[index] = std::move(value);
        
        raw_size++;
        return _data+index;
    }
    
    template<typename T>
    T* vector<T>::insert(const T* pos, size_t count, const T& value) {
        if(pos == end()) {
            push_back(value);
            return rend();
        }
        // If we work out index after reserve pointer distance is wrong!
        size_t index = std::distance(cbegin(), pos);
        const size_t required_size = raw_size + count;
        if(required_size > _capacity) {
            if(required_size > next_capacity()) {
                reserve(required_size);
            } else {
                reserve(next_capacity());
            }
        }
        memmove(_data+index+count, _data+index, (raw_size-index)*sizeof(T));
        for(size_t i=0; i<count; i++) { 
            _data[index+i] = value;
        }
        raw_size += count;
        return _data+index;
    }

    template<typename T>
    T* vector<T>::insert(const T* pos, const T* first, const T* last) {
        size_t index = std::distance(cbegin(), pos);
        size_t n_elem = std::distance(first, last);
        for(size_t i=0; i<n_elem; i++) {
            insert(begin()+index+i, first[i]);
        }
        return begin() + index;
    }

    template<typename T>
    T* vector<T>::insert(const T* pos, std::initializer_list<T> il) {
        return insert(pos, il.begin(), il.end());
    }
    
    template<typename T>
    T* vector<T>::erase(const T* pos) {
        const size_t index = std::distance(cbegin(), pos);
        _data[index].~T();
        if(index == raw_size - 1) {
            raw_size--;
            return rbegin();
        } else {
            memmove(_data+index, _data+index+1, (raw_size-index)*sizeof(T));
            raw_size--;
            return _data+index;
        }
    }

    template<typename T>
    T* vector<T>::erase(const T* first, const T* last) {
        const size_t len = std::distance(first, last);
        const size_t index = std::distance(cbegin(), first);
        const size_t end_index = std::distance(cbegin(), last);
        
        T* pos = &_data[index];
        
        if(first != last) {
            for( ; pos != last; pos++) {
                pos->~T();
            }
            memmove(pos, last, raw_size - end_index);
            raw_size -= len;
        }

        return pos;
    }

    template<typename T>
    void vector<T>::pop_back() {
        if(!empty()) {
            _data[raw_size - 1]->~T();
            raw_size--;
        }
    }

    template<typename T>
    void vector<T>::resize(size_t count) {
        resize(count, T());
    }
    
    template<typename T>
    void vector<T>::resize(size_t count, const T& value) {
        if(count < raw_size) {
            for(size_t i=count; i<raw_size; i++) {
                _data[i]->~T();
                _data[i] = nullptr;
            }
            raw_size = count;
        } else {
            if(_capacity < count) {
                reserve(count);
            }
            for(size_t i=raw_size; i<count; i++) {
                push_back(value);
            }
        }
    }

    template<typename T>
    T& vector<T>::operator[](size_t i) {
        return at(i);
    }

    template<typename T>
    const T& vector<T>::operator[](size_t i) const {
        return at(i);
    }

    template<typename T>
    T& vector<T>::at(size_t i) {
        if(!(i < raw_size)) {
            throw std::out_of_range("Attempted to access element out of range");
        }
        return _data[i];
    }
    
    template<typename T>
    const T& vector<T>::at(size_t i) const {
        if(!(i < raw_size)) {
            throw std::out_of_range("Attempted to access element out of range");
        }
        return _data[i];
    }

    template<typename T>
    const T& vector<T>::front() const {
        return at(0);
    }

    template<typename T>
    T& vector<T>::front() {
        return at(0);
    }

    template<typename T>
    const T& vector<T>::back() const {
        return at(raw_size-1);
    }

    template<typename T>
    T& vector<T>::back() {
        return at(raw_size-1);
    }

    template<typename T> 
    size_t vector<T>::size() const noexcept {
        return raw_size;
    }

    template<typename T>
    size_t vector<T>::capacity() const noexcept {
        return _capacity;
    }

    template<typename T>
    size_t vector<T>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max();
    }

    template<typename T>
    bool vector<T>::empty() const noexcept {
        return raw_size == 0;
    }

    template<typename T> 
    void vector<T>::clear() {
        delete[] _data;
        raw_size = 0;
    }

    template<typename T>
    void vector<T>::shrink_to_fit() {
        T* new_data = new T[raw_size];
        memcpy(new_data, _data, raw_size*sizeof(T));
        delete[] _data;
        _data = new_data;
        _capacity = raw_size;
    }
    
    template<typename T>
    void vector<T>::swap(vector<T>& other) noexcept {
        T* tmp_data = _data;
        _data = other._data;
        other._data = tmp_data;

        size_t tmp_size = raw_size;
        raw_size = other.raw_size;
        other.raw_size = tmp_size;

        tmp_size = _capacity;
        _capacity = other._capacity;
        other._capacity = tmp_size;
    }

    template<typename T>
    T* vector<T>::data() noexcept {
        return _data;
    }

    template<typename T>
    const T* vector<T>::data() const noexcept {
        return _data;
    }

    template<typename T>
    T* vector<T>::begin() noexcept {
        return data();
    }

    template<typename T>
    const T* vector<T>::begin() const noexcept {
        return cbegin();
    }

    template<typename T>
    const T* vector<T>::cbegin() const noexcept {
        return data();
    }

    template<typename T>
    T* vector<T>::end() noexcept {
        return _data + raw_size;
    }

    template<typename T>
    const T* vector<T>::end() const noexcept {
        return cend();
    }

    template<typename T>
    const T* vector<T>::cend() const noexcept {
        return _data + raw_size;
    }

    template<typename T>
    T* vector<T>::rbegin() noexcept {
        return data() - 1;
    }

    template<typename T>
    const T* vector<T>::rbegin() const noexcept {
        return crbegin();
    }

    template<typename T>
    const T* vector<T>::crbegin() const noexcept {
        return data() - 1;
    }

    template<typename T>
    T* vector<T>::rend() noexcept {
        return _data + raw_size - 1;
    }

    template<typename T>
    const T* vector<T>::rend() const noexcept {
        return crend();
    }

    template<typename T>
    const T* vector<T>::crend() const noexcept {
        return _data + raw_size - 1;
    }
}



#endif
