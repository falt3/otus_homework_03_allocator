#pragma once

#include <memory>
#include <vector>
#include <algorithm>

//-------------------------------------------------------------------
/**
 * @brief Класс обертки аллокатора для передачи размера
 * 
 * @tparam SIZE Количество элементов для котроых надо выделить
 */
template <size_t SIZE> 
struct MyAllocator_Wrapper {

    /**
     * @brief 
     * 
     * @tparam T 
     */
    template <class T>
    struct MyAllocator_11
    {
        using value_type =  T;

        MyAllocator_11() noexcept {}
        template <class U>
        // MyAllocator_11(const MyAllocator_11<U>& A2) noexcept {}
        MyAllocator_11(const MyAllocator_11<T>& A2) noexcept = delete;
        ~MyAllocator_11() {
            if (m_pointer) {
                // std::cout << "Allocator:deallocate "<< " " << m_pointer << std::endl;
                ::operator delete(m_pointer);
                m_pointer = nullptr;
            }
        }
        /**
         * @brief Возвращает количество свободной памяти
         * 
         * @return std::size_t 
         */
        std::size_t countFreeMemory() { return SIZE - m_countMemoryBusy; }
        
        /**
         * @brief Возвращает true, если адрес лежит в выделенной облати аллокатора
         * 
         * @param ptr 
         * @return true 
         * @return false 
         */
        bool checkMemory(T* ptr) { 
            if (ptr >= m_pointer) {
                std::size_t j = ptr - m_pointer;
                return j < SIZE;
            }
            else 
                return false;
        }

        T* allocate(std::size_t n)
        {
            if (n > 1) throw "error";
            // std::cout << "allocate: " << n << " " << m_pointer << " " << SIZE << " " << sizeof(T) << "\n";
            if (m_pointer == nullptr) { // выделить память
                m_pointer = static_cast<T*>(::operator new(SIZE * sizeof(T)));
                if (m_mapMemoryBusy.size() == 0)
                    m_mapMemoryBusy.resize(SIZE, 0);
                // std::cout << "Allocator:allocate " << m_pointer << " " << SIZE*sizeof(T) << " " << m_mapBusyMemory.size() << std::endl;
            }
            if (m_countMemoryBusy == SIZE) { // память вся использована -> исключение
                // std::cout << "Allocator:allocate exception \n";
                throw "error";
            }
            for (std::size_t j = 0; j < SIZE; ++j) {
                if (m_mapMemoryBusy[j] == 0) {
                    m_mapMemoryBusy[j] = 1;
                    ++m_countMemoryBusy;
                    // std::cout << "  return  " << m_countMemoryBusy << " " << j << "\t" << (m_pointer + j) << "\n";
                    return m_pointer + j;
                }
            }
            throw "error";
        }

        void deallocate(T* ptr, std::size_t /*n*/)
        {
            if (ptr >= m_pointer) {
                std::size_t j = ptr - m_pointer;
                // std::cout << "Allocator:deallocate: j:" << j << std::endl;
                if (j < SIZE) {
                    m_mapMemoryBusy[j] = 0;
                    m_countMemoryBusy--;
                }
            }
        }

        // using propagate_on_container_copy_assignment = std::true_type;
        // using propagate_on_container_move_assignment = std::true_type;
        // using prooagate_on_container_swap = std::true_type;

    private:
        T*          m_pointer   = nullptr;      // Указатель на начало выделенной памяти
        std::size_t m_countMemoryBusy = 0;      // Количество занятых ячеек памяти
        std::vector<char> m_mapMemoryBusy;      // Карта занятых ячеек памяти (0 - свободная, 1 - занятая)
    };
};

template <class T, size_t TS, class U, size_t US>
constexpr bool operator== (const typename MyAllocator_Wrapper<TS>::MyAllocator_11<T>& a1, 
                           const typename MyAllocator_Wrapper<US>::MyAllocator_11<U>& a2) noexcept
{
    return true;
}

template <class T, size_t TS, class U, size_t US>
constexpr bool operator!= (const typename MyAllocator_Wrapper<TS>::MyAllocator_11<T>& a1, 
                           const typename MyAllocator_Wrapper<US>::MyAllocator_11<U>& a2) noexcept
{
    return false;
}

template <typename T, size_t SIZE = 100>
using MyAllocator = typename MyAllocator_Wrapper<SIZE>::MyAllocator_11<T>;

//-------------------------------------------------------------------
/**
 * @brief Класс 
 * 
 * @tparam SIZE 
 */
template <size_t SIZE> 
struct BlockAllocator_Wrapper {

    template <class T>
    struct BlockAllocator {
        using value_type =  T;

        BlockAllocator() noexcept {}
        template <class U>
        BlockAllocator(const BlockAllocator<U>& A2) noexcept = delete;
        BlockAllocator(const BlockAllocator<T>& A2) noexcept = delete;

        T* allocate(std::size_t n) {
            for (std::size_t i = 0; i < pages.size(); ++i) {
                if (pages[i]->countFreeMemory()) {
                    return pages[i]->allocate(n);
                }
            }
            auto ptr = std::unique_ptr<MyAllocator<T, SIZE>>(new MyAllocator<T, SIZE>);
            pages.push_back(std::move(ptr));
            return pages.back()->allocate(n);
        }

        void deallocate(T* ptr, std::size_t n) {
            auto it = std::find_if(pages.cbegin(), pages.cend(), [ptr](auto& el) {
                return el->checkMemory(ptr);
            });
            if (it != pages.end()) {
                (*it)->deallocate(ptr, n);
                if ((*it)->countFreeMemory() == SIZE) 
                    pages.erase(it);
            }
        }

        size_t countPages() { return pages.size(); }
    private:
        std::vector<std::unique_ptr<MyAllocator<T, SIZE>>> pages;
    };
};


template <class T, size_t TS, class U, size_t US>
constexpr bool operator== (const typename BlockAllocator_Wrapper<TS>::BlockAllocator<T>& a1, 
                           const typename BlockAllocator_Wrapper<US>::BlockAllocator<U>& a2) noexcept
{
    return true;
}

template <class T, size_t TS, class U, size_t US>
constexpr bool operator!= (const typename BlockAllocator_Wrapper<TS>::BlockAllocator<T>& a1, 
                           const typename BlockAllocator_Wrapper<US>::BlockAllocator<U>& a2) noexcept
{
    return false;
}

template <typename T, size_t SIZE = 100>
using BlockAllocator = typename BlockAllocator_Wrapper<SIZE>::BlockAllocator<T>;