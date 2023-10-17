#pragma once

#include <memory>
#include <iostream>
#include <cassert>



template <typename T, typename Allocator = std::allocator<T>>
class MyList {
    struct Node {
        Node* next  = nullptr;
        Node* prev  = nullptr;
        T data;
        Node(const T& x) : data(x) {} 
    };    
    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using NodeAllocatorTraits = typename std::allocator_traits<Allocator>::template rebind_traits<Node>;    

public:
    struct iterator {
        Node* m_current;

        iterator(Node *node) : m_current(node) {}
        iterator& operator++() {
            m_current = m_current->next;
            return *this;
        }
        T operator*() { return m_current->data; }
        bool operator==(iterator other) { return m_current == other.m_current; }
        bool operator!=(iterator other) { return m_current != other.m_current; }
    };

    MyList() noexcept {};
    MyList(Allocator alloc) : m_allocator(alloc) {}
    MyList(MyList& other) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            push_back(*it);
        }
    }
    MyList(MyList&& other) = delete;
    MyList& operator=(const MyList& other) = delete;
    MyList& operator=(MyList&& other) = delete;
    ~MyList() {
        for (auto it = begin(); it != end(); it = erase(it)) {}
    }

    
    void push_back (const T& x);
    T pop_back();
    T pop_front();   
    iterator erase(iterator pos) {
        Node *node = pos.m_current;
        if (node->prev)
            node->prev->next = node->next;
        Node* nodeNext = node->next;
        if (node->next)
            node->next->prev = node->prev;
        std::allocator_traits<NodeAllocator>::destroy(m_allocator, node);
        m_allocator.deallocate(node, 1);
        m_size--;
        return nodeNext;
    }
    int size() { return m_size; }
    bool empty() { return m_first == nullptr; }

    iterator begin() { return m_first; }
    iterator end() { return nullptr; }

private:
    Node* m_first   = nullptr;
    Node* m_last    = nullptr;

    NodeAllocator m_allocator;
    int m_size = 0;
};


template <typename T, typename Allocator>
void MyList<T, Allocator>::push_back(const T &x)
{
    Node* newNode = NodeAllocatorTraits::allocate(m_allocator, 1);  // NOLINT
    NodeAllocatorTraits::construct(m_allocator, newNode, x);
    if (m_size == 0)            
        m_first = newNode;
    else {
        m_last->next = newNode;
        newNode->prev = m_last;
    }
    m_last = newNode;
    m_size++;
}


template <typename T, typename Allocator>
T MyList<T, Allocator>::pop_back()
{
    assert(m_size && "pop_back: Size = 0");
    if (m_size > 0) {
        T a = m_last->data;
        auto ptr = m_last;
        if (m_last->prev) {
            m_last = m_last->prev;
            m_last->next = nullptr;
        }
        std::allocator_traits<NodeAllocator>::destroy(m_allocator, ptr);

        m_allocator.deallocate(ptr, 1);
        m_size--;
        return a;
    }
    return 0;
}


template <typename T, typename Allocator>
T MyList<T, Allocator>::pop_front()
{
    assert(m_size && "pop_front: Size = 0");
    if (m_size > 0) {
        T a = m_first->data;
        auto ptr = m_first;
        if (m_first->next) {
            m_first = m_first->next;
            m_first->prev = nullptr;
        }
        std::allocator_traits<NodeAllocator>::destroy(m_allocator, ptr);
        m_allocator.deallocate(ptr, 1);            
        m_size--;
        return a;
    }
    return 0;
} 



