// It nearly satisfy the requirements of an allocator and
// it still cannot run correctly. Don't use them!
// file: ec_alloc.h
#ifndef ECALLOC_H
#define ECALLOC_H

#include <climits>   // for UINT_MAX
#include <cstddef>   // for ptrdiff_t, size_t
#include <iostream>  // for cerr
#include <new>       // for placement new

namespace ec_alloc {

template <class T>
inline T *_allocate(ptrdiff_t size, T *) {
  set_new_handler(0);
  T *tmp = (T *)(::operator new T(size * sizeof(T)));
  if (tmp == 0) {
    cerr << "out of memory" << endl;
    exit(1);
  }
  return tmp;
}

template <class T>
inline void _deallocate(T *buffer) {
  ::operator delete(buffer);
}

template <class T1, class T2>
inline _construct(T1 *p, const T2 &value) {
  new (p) T1(value);
}

template <class T>
inline void _destory(T *ptr) {
  ptr->~T();
}

template <class T>
class allocator {
 public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  template <class U>
  struct rebind {
    typedef allocator<U> other;
  }

  // default constructor
  // allocator();
  // destructor
  // ~allocator();
  // copy constructor
  // allocator(const allocator &x) { _construct(this, x); }

  pointer
  allocate(size_type n, const void *hint = 0) {
    return _allocate((difference_type)n, (pointer)0);
  }
  // return a non-const object's address. a.address(x) is the same as &x
  pointer address(reference x) const { return (pointer)&x; }
  // return a non-const object's address. a.address(x) is the same as &x
  const_pointer address(const_reference x) const { return (const_pointer)&x; }
  // allocate space for n T-type objects.
  pointer allocate(size_type n) { return _allocate(n); }
  // release allocated space
  void deallocate(pointer p) { _deallocate(p); }
  // return the max size we can allocate successfully
  void construct(pointer p, const_reference x) { _construct(p, x); }
  // the same as p->~T()
  void destory(pointer p) { _destory(p); }
  // return the max size which can be allocated
  size_type max_size() const { return size_type(UINT_MAX / sizeof(T)); }
};

}  // namespace ECalloc
#endif
