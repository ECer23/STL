#ifndef MEMORY_MANAGER_HPP_
#define MEMORY_MANAGER_HPP_

#define __STL_NOTHROW throw()
#include <iostream>
#include <limits>
#include <malloc.h>

template <class _Tp> class MemoryManager {
public:
  // type definitions
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Tp *pointer;
  typedef const _Tp *const_pointer;
  typedef _Tp &reference;
  typedef const _Tp &const_reference;
  typedef _Tp value_type;

  // rebind allocator to type U
  template <class _Tp1> struct rebind { typedef MemoryManager<_Tp1> other; };

  // nothing to do because the allocator has no state
  MemoryManager() __STL_NOTHROW : _M_pool(NULL) {}
  MemoryManager(const MemoryManager &) __STL_NOTHROW : _M_pool(NULL) {}
  template <class _Tp1>
  MemoryManager(const MemoryManager<_Tp1> &) __STL_NOTHROW {}
  ~MemoryManager() __STL_NOTHROW {
    while (_M_pool->next != NULL) {
      Node *p = _M_pool->next;
      _M_pool->next = p->next;
      delete p;
    }
    delete _M_pool;
  }

  // return address of values
  pointer address(reference __x) const { return &__x; }
  const_pointer address(const_reference __x) const { return &__x; }

  _Tp *allocate(size_type __n, const void * = 0) {
    if (_M_pool == NULL) {
      Node *temp;
      void *p = operator new(sizeof(_Tp));
      temp = reinterpret_cast<Node *>(p); // why sizeof(_Tp)?
      temp->next = _M_pool;
      _M_pool = temp;
    }
    pointer result = reinterpret_cast<pointer>(_M_pool);
    _M_pool = _M_pool->next;
    return result;
  }

  // __p is not permitted to be a null pointer.
  void deallocate(pointer __p, size_type __n) {
    if (__p != NULL) {
      reinterpret_cast<Node *>(__p)->next = _M_pool;
      _M_pool = reinterpret_cast<Node *>(__p);
    }
  }

  size_type max_size() const __STL_NOTHROW {
    // return the maximum number of elements that can be allocated
    return std::numeric_limits<std::size_t>::max() / sizeof(_Tp);
  }

  void construct(pointer __p, const _Tp &__val) {
    // intialize memory with placement new
    new ((void *)__p) _Tp(__val);
  }

  // destory elements of initialized storage p
  void destroy(pointer __p) {
    // destory objects by calling their destructor
    __p->~_Tp();
  }

private:
  // defined your private filed
  union Node {
    union Node *next;
    value_type element;
  };
  Node *_M_pool;
};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator==(const MemoryManager<T1> &,
                const MemoryManager<T2> &) __STL_NOTHROW {
  return true;
}
template <class T1, class T2>
bool operator!=(const MemoryManager<T1> &,
                const MemoryManager<T2> &) __STL_NOTHROW {
  return false;
}

#endif
