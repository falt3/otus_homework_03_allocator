#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <map>



template <class T, class Allocator = std::allocator<T> >
class my_vector {
public:
    my_vector() noexcept {};
    my_vector(Allocator alloc) : m_allocator(alloc) {}
    
    void push_back(const T& x)
    {
        if (m_size == capacity)
        {
            std::size_t capacity_old = capacity;
            capacity = capacity * 2 + 1;
            std::cout << "my_vector::allocate " << m_size << " " << capacity << " " << std::endl;
            T* newData = std::allocator_traits<Allocator>::allocate(m_allocator, capacity);
            std::cout << "my_vector::allocate " << newData << std::endl;
            if (data) {
                std::copy(data, data + capacity_old * sizeof(T), newData); //naive
                std::cout << "my_vector::deallocate " << capacity_old << " " << data << std::endl;
                std::allocator_traits<Allocator>::deallocate(m_allocator, data, capacity_old);
            }
            std::swap(newData, data);
        }

        std::allocator_traits<Allocator>::construct(m_allocator, data + m_size*sizeof(T), x);
        ++m_size;
    }

    std::size_t size() { return m_size; }

    T& operator[] (int i) { return *(data + i*sizeof(T)); }


private:
    std::size_t m_size = 0;
    std::size_t capacity = 0;
    T* data = nullptr;

    Allocator m_allocator;
};