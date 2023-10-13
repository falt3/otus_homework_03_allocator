#include <memory>
#include <vector>
#include <algorithm>


//-------------------------------------------------------------------

template <typename T>
struct MyAllocator_11_simple {
    using value_type = T;

    MyAllocator_11_simple() noexcept {}
    
    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t /*n*/) {
        ::operator delete(p);
    }
};

//-------------------------------------------------------------------

template <class T>
struct MyAllocator_11
{
    using value_type =  T;

    MyAllocator_11(std::size_t maxSize) noexcept : m_maxSize(maxSize)
    {
        std::cout << "Allocator:constructor " << m_maxSize << "\n";
    }
    template <class U>
    MyAllocator_11(const MyAllocator_11<U>& A2) noexcept
    {
        std::cout << "Allocator:constructor_copy \n";
        this->m_maxSize = A2.m_maxSize;
    }
    MyAllocator_11(const MyAllocator_11<T>& A2) noexcept
    {
        std::cout << "Allocator:constructor_copy_T\n";
        m_maxSize = A2.m_maxSize;
        if (A2.m_pointer != nullptr) {
            allocate(A2.m_busyCount);
            // std::copy(A2.m_pointer, A2.m_pointer + A2.m_busy, m_pointer);
            std::copy((char *)A2.m_pointer, (char *)(A2.m_pointer + A2.m_busyCount), (char *)m_pointer);
        }
    }    
    ~MyAllocator_11() {
        if (m_pointer) {
            std::cout << "Allocator:deallocate "<< " " << m_pointer << std::endl;
            ::operator delete(m_pointer);
            m_pointer = nullptr;
        }
    }
    std::size_t countFreeMemory() { return m_maxSize - m_busyCount; }
    bool checkMemory(T* ptr) { 
        if (ptr >= m_pointer) {
            std::size_t j = ptr - m_pointer;
            return j < m_maxSize;
        }
        else 
            return false;
    }

    T* allocate(std::size_t n)
    {
        if (n > 1) throw "error";
        // std::cout << "allocate: " << n << " " << m_pointer << " " << m_maxSize << " " << sizeof(T) << "\n";
        if (m_pointer == nullptr) { // выделить память
            m_pointer = static_cast<T*>(::operator new(m_maxSize * sizeof(T)));
            if (m_mapBusyMemory.size() == 0)
                m_mapBusyMemory.resize(m_maxSize, 0);
            // std::cout << "Allocator:allocate " << m_pointer << " " << m_maxSize*sizeof(T) << " " << m_mapBusyMemory.size() << std::endl;
        }
        if (m_busyCount == m_maxSize) { // память вся использована -> исключение
            // std::cout << "Allocator:allocate exception \n";
            throw "error";
        }
        for (std::size_t j = 0; j < m_maxSize; ++j) {
            if (m_mapBusyMemory[j] == 0) {
                m_mapBusyMemory[j] = 1;
                ++m_busyCount;
                // std::cout << "  return  " << m_busyCount << " " << j << "\t" << (m_pointer + j) << "\n";
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
            if (j < m_maxSize) {
                m_mapBusyMemory[j] = 0;
                m_busyCount--;
            }
        }
    }

    std::size_t m_maxSize;

    // using propagate_on_container_copy_assignment = std::true_type;
    // using propagate_on_container_move_assignment = std::true_type;
    // using prooagate_on_container_swap = std::true_type;

private:
    std::size_t m_busyCount = 0;
    T*          m_pointer   = nullptr;
    std::vector<char> m_mapBusyMemory;
};


template <class T, class U>
constexpr bool operator== (const MyAllocator_11<T>& a1, const MyAllocator_11<U>& a2) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!= (const MyAllocator_11<T>& a1, const MyAllocator_11<U>& a2) noexcept
{
    return false;
}

//-------------------------------------------------------------------
/**
 * @brief 
 * 
 * @tparam T 
 */
template <class T>
struct BlockAllocator {
    using value_type =  T;

    BlockAllocator() noexcept {}
    // template <class U>
    // BlockAllocator(const BlockAllocator<U>& A2) noexcept {}
    // BlockAllocator(const BlockAllocator<T>& A2) noexcept {}

    T* allocate(std::size_t n) {
        for (std::size_t i = 0; i < pool.size(); ++i) {
            if (pool[i]->countFreeMemory()) {
                return pool[i]->allocate(n);
            }
        }
        auto ptr = std::unique_ptr<MyAllocator_11<T>>(new MyAllocator_11<T>(10));
        pool.push_back(std::move(ptr));
        return pool.back()->allocate(n);
    }

    void deallocate(T* ptr, std::size_t n) {
        auto it = std::find_if(pool.cbegin(), pool.cend(), [ptr](auto& el) {
            return el->checkMemory(ptr);
        });
        if (it != pool.end()) {
            (*it)->deallocate(ptr, n);
            if ((*it)->countFreeMemory() == 10) 
                pool.erase(it);
        }
    }
private:
    std::vector<std::unique_ptr<MyAllocator_11<T>>> pool;
};


template <class T, class U>
constexpr bool operator== (const BlockAllocator<T>& a1, const BlockAllocator<U>& a2) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!= (const BlockAllocator<T>& a1, const BlockAllocator<U>& a2) noexcept
{
    return false;
}