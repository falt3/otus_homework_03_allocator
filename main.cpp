#include <iostream>
#include <map>
#include <memory>

//*****************************************************************

template <class T>
struct Allocator_11
{
    using value_type =  T;

    Allocator_11(std::size_t maxSize) noexcept : m_maxSize(maxSize)
    {
        std::cout << "Allocator:constructor " << m_maxSize << "\n";
    }
    template <class U>
    Allocator_11(const Allocator_11<U>& A2) noexcept
    {
        std::cout << "Allocator:constructor_copy \n";
        this->m_maxSize = A2.m_maxSize;
    }
    Allocator_11(const Allocator_11<T>& A2) noexcept
    {
        std::cout << "Allocator:constructor_copy_T\n";
        m_maxSize = A2.m_maxSize;
        if (A2.m_pointer != nullptr) {
            allocate(A2.m_busy);
            // std::copy(A2.m_pointer, A2.m_pointer + A2.m_busy, m_pointer);
            std::copy((char *)A2.m_pointer, (char *)(A2.m_pointer + A2.m_busy), (char *)m_pointer);
        }
    }    

    T* allocate(std::size_t n)
    {
        if (m_pointer == nullptr) { // выделить память
            m_pointer = static_cast<T*>(::operator new(m_maxSize*sizeof(T)));
            std::cout << "Allocator:allocate " << m_pointer << " " << m_maxSize*sizeof(T) << std::endl;
        }
        if (m_busy + n > m_maxSize) { // память вся использована -> исключение
            std::cout << "Allocator:allocate exception \n";
            throw "error";
        }
        m_busy += n;
        std::cout << "      " << m_busy - n << "\t" << m_pointer + (m_busy - n) << "\n";
        return m_pointer + (m_busy - n);
    }

    void deallocate(T*, std::size_t n)
    {
        // std::ignore = p;
        m_busy -= n;
        if (m_busy == 0) {
            std::cout << "Allocator:deallocate "<< " " << m_pointer << std::endl;
            ::operator delete(m_pointer);
            m_pointer = nullptr;
        }
    }

    std::size_t m_maxSize;

    // using propagate_on_container_copy_assignment = std::true_type;
    // using propagate_on_container_move_assignment = std::true_type;
    // using prooagate_on_container_swap = std::true_type;

private:
    std::size_t m_busy      = 0;
    T*          m_pointer   = nullptr;
};


template <class T, class U>
constexpr bool operator== (const Allocator_11<T>& a1, const Allocator_11<U>& a2) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!= (const Allocator_11<T>& a1, const Allocator_11<U>& a2) noexcept
{
    return false;
}

//*****************************************************************

constexpr int factorial(int k) {
    int res = 1;
    for (int i = 2; i <= k; ++i)
        res *=i;
    return res;
}

//*****************************************************************

int main()
{
    std::map<int, int> m;
    for (int i = 0; i <= 9; ++i)
        m.insert({i, factorial(i)});

    using TA = Allocator_11<std::pair<const int, int>>;
    TA alloc1(10);
    // std::map<int, int, std::less<int>, TA> m1(Allocator_11<TA>(10));
    std::map<int, int, std::less<int>, TA> m1(alloc1);
    for (int i = 0; i <= 9; ++i)
        m1.insert({i, factorial(i)});
    for (auto el: m)
        std::cout << el.first << " " << el.second << std::endl;

    // Allocator_11<int> ann(10);
    // int* z = ann.allocate(1);
    // *z = 33;
    // auto bob = ann;
    // int* a = ann.allocate(1);
    // *a = 0;
    // int* b = bob.allocate(1);
    // *b = 42;
    // std::cout << *z << " " << *a << std::endl;
    // std::cout << *(b-1) << " " << *b << std::endl;

    return 0;
}
