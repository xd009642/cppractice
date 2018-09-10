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
        vector(size_t count, const T& value = T());

        vector(vector<T>::const_iterator first, vector<T>::const_iterator last);

        vector(std::initializer_list<T> l);

        ~vector();

        vector& operator=(std::initializer_list<T> il);
            
        void reserve(size_t capacity);

        void push_back(const T& value);

        void clear();

        reference operator[](size_t index);

        reference at(size_t index);

        const_reference at(size_t index) const;

        size_t size() const noexcept;

        bool empty() const noexcept;

        size_t max_size() const noexcept;
    protected:

    private:
        //! Current size of the vector
        size_t raw_size;
        //! Current capacity 
        //!(capacity >= size to prevent continuously allocating)
        size_t capacity;
        //! Array containing the data
        T* data;
    };



    template<typename T>
    vector<T>::vector():
    raw_size(0),
    capacity(0),
    data(nullptr) {
    }

    template<typename T>
    vector<T>::vector(size_t count, const T& value):vector() {
        reserve(count);
        for(size_t i=0; i<count; i++) {
            push_back(value);
        }
    }

    template<typename T>
    vector<T>::vector(vector<T>::const_iterator begin, vector<T>::const_iterator end):vector(){
        reserve(std::distance(begin, end));
        for(auto it=begin; it != end; it++) {
            push_back(*it);
        }
    }

    template<typename T>
    vector<T>::vector(std::initializer_list<T> il):vector(il.begin(), il.end()){
    }

    template<typename T>
    vector<T>::~vector() {
        clear();
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
        if(cap <= capacity) {
            return;
        }
        T* new_data = new T[cap];
        if(data != nullptr) {
            memcpy(new_data, data, raw_size);
            delete[] data;
        }
        data = new_data;
        capacity = cap;
    }

    template<typename T> 
    void vector<T>::push_back(const T& value) {
        if((raw_size + 1) > capacity) {
            // Double rate growth factor just to be simple
            size_t new_size = capacity==0 ? 1 : capacity * 2; 
            reserve(new_size);
        }
        data[raw_size] = value;
        raw_size++;
    }

    template<typename T>
    T& vector<T>::operator[](size_t i) {
        return at(i);
    }

    template<typename T>
    T& vector<T>::at(size_t i) {
        if(!(i < raw_size)) {
            throw std::out_of_range("Attempted to access element out of range");
        }
        return data[i];
    }
    
    template<typename T>
    const T& vector<T>::at(size_t i) const {
        if(!(i < raw_size)) {
            throw std::out_of_range("Attempted to access element out of range");
        }
        return data[i];
    }

    template<typename T> 
    size_t vector<T>::size() const noexcept {
        return raw_size;
    }

    template<typename T>
    size_t vector<T>::max_size() const noexcept {
        std::numeric_limits<size_t>::max();
    }

    template<typename T>
    bool vector<T>::empty() const noexcept {
        return raw_size == 0;
    }

    template<typename T> 
    void vector<T>::clear() {
        delete[] data;
        raw_size = 0;
    }
}



#endif
