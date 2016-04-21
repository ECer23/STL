// file: ec_alloc.h
#ifndef ECALLOC_H
#define ECALLOC_H

#include <cstddef> // for ptrdiff_t, size_t
#include <new>     // for placement new

namespace ec_alloc {

template <class T> inline T *_allocate(size_t size, T *) {
  T *tmp;
  tmp = new T(size * sizeof(T));
  return tmp;
}

template <class T> inline void _deallocate(T *buffer) { delete (buffer); }

template <class T1, class T2> inline _construct(T1 *p, const T2 &value) {
  new (p) T1(value);
}

template <class T> class allocator {
public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;

  // default constructor
  allocator();
  // destructor
  ~allocator();
  // copy constructor
  allocator(const allocator &x) { _construct(this, x); }
  // return a non-const object's address. a.address(x) is the same as &x
  pointer address(reference x) const { return (pointer)&x; }
  // return a non-const object's address. a.address(x) is the same as &x
  const_pointer address(const_reference x) const { return (const_pointer)&x; }
  // allocate space for n T-type objects.
  pointer allocate(size_type n) { _allocate(n); }
  // release allocated space
  void deallocate(pointer p) { _deallocate(p); }
  // return the max size we can allocate successfully
  void construct(pointer p, const T &x) { _construct(p, x); }
  // the same as p->~T()
  void destory(pointer p) { p->~T(); }
};

} // namespace ECalloc
#endif