#ifndef __VECTOR_H__
#define __VECTOR_H__

#define _a Alloc()
// allocate(size_type n) 申请这么n个value_type的内存
// construct(pointer p, value_type value) 构造指针位置的内存为值value
// deallocate(pointer p, std::size_type n) 只释放对象本身的内存，n代表大小（还给系统或者扔进内存池）
// destroy(pointer p) 只释放由p指向的对象申请的动态内存

#include "base.h"
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class myVector : public Base {
public:
  // Constructor
  // 基本思路是allocate所需内存，把size和capacity设置为同样大小，然后进行构造（赋值）
  // 容量只有内存，不构造

  // 默认构造把容量设置为1，大小设置为0。申请一块内存，不对他进行赋值
  myVector() {
    _data = _a.allocate(1);  // 用默认的allocator申请内存
    _size = 0;
    _capacity = 1;
  }

  // a是可选的allocater
  myVector(const std::size_t &size, const T &val, Alloc a = Alloc()) {
    _data = a.allocate(size);  // 申请size大小的内存
    for (std::size_t i = 0; i < size; ++i)
      a.construct(_data + i, val);  // 构造
    _size = _capacity = size;  // 容量也为size大小
  }

  template <typename InputIterator>
  myVector(InputIterator begin, InputIterator end, Alloc a = Alloc()) {
    _size = _capacity = end - begin;  // 这个是迭代器（指针）相减
    _data = a.allocate(_size);
    std::size_t cnt = 0;
    for (InputIterator it = begin; it != end; ++it)
      a.construct(_data + (cnt++), *it);
  }

  myVector(const myVector &other) {
    _size = other._size;
    _capacity = other._capacity;
    _data = _a.allocate(_capacity);  // 申请的内存申请容量大小相同
    for (std::size_t i = 0; i < _size; ++i)  // 但是构造的时候不构造容量部分
      _a.construct(_data + i, other._data[i]);
  }

  // Destructor
  // 基本思路是先摧毁（挨个），再释放内存（一次性）
  ~myVector() {
    for (std::size_t i = 0; i < _size; ++i)
      _a.destroy(_data + i);
    if (_capacity > 0)
      _a.deallocate(_data, _capacity);
  }

  // Copy Operator
  myVector &operator=(const myVector &other) {
    if (&other != this) {
      std::size_t i;
      for (i = 0; i < _size; ++i)
        _a.destroy(_data + i);
      if (_capacity > 0)
        _a.deallocate(_data, _capacity);
      _size = other._size;
      _capacity = other._capacity;
      _data = _a.allocate(_capacity);
      for (i = 0; i < _size; ++i)
        _a.construct(_data + i, other._data[i]);
    }
    return *this;
  }

  // Iterator
  typedef T *iterator;
  typedef const T *const_iterator;

  inline iterator begin() { return _data; }
  inline const_iterator begin() const { return _data; }
  inline iterator end() { return _data + _size; }
  inline const_iterator end() const { return _data + _size; }

  // Capacity
  inline std::size_t size() const { return _size; }
  void resize(const std::size_t &newSize) {
    std::size_t i;
    if (newSize <= _size) {
      for (i = newSize; i < _size; ++i)
        _a.destroy(_data + i);
    } else {
      if (newSize > _capacity) {
        std::size_t newCapacity = _capacity * 2;
        while (newSize > newCapacity)
          newCapacity *= 2;
        reserve(newCapacity);
      }
      for (i = _size; i < newSize; ++i)
        _a.construct(_data + i, T());
    }
    _size = newSize;
  }
  void resize(const std::size_t &newSize, const T &val) {
    std::size_t i;
    if (newSize <= _size) {
      for (i = newSize; i < _size; ++i)
        _a.destroy(_data + i);
    } else {
      if (newSize > _capacity) {
        std::size_t newCapacity = _capacity * 2;
        while (newSize > newCapacity)
          newCapacity *= 2;
        reserve(newCapacity);
      }
      for (i = _size; i < newSize; ++i)
        _a.construct(_data + i, val);
    }
    _size = newSize;
  }
  inline std::size_t capacity() const { return _capacity; }
  inline bool empty() const { return _size == 0; }

  // reserve只改变了容器的容量，不改变容器大小
  // 如果容量变小，就把多的删掉就好
  void reserve(const std::size_t &newCapacity) {
    T *temp = _a.allocate(newCapacity);
    std::size_t s;
    if (newCapacity > _size)
      s = _size;
    else
      s = newCapacity;
    for (std::size_t i = 0; i < s; ++i) {
      _a.construct(temp + i, _data[i]);
      _a.destroy(_data + i);
    }
    _a.deallocate(_data, _capacity);
    _capacity = newCapacity;
    if (newCapacity < _size)
      _size = _capacity;
    _data = temp;
  }

  // Element Access
  inline T &operator[](const std::size_t &index) { return _data[index]; }
  inline const T &operator[](const std::size_t &index) const {
    return _data[index];
  }
  inline T &front() { return _data[0]; }
  inline const T &front() const { return _data[0]; }
  inline T &back() { return _data[_size - 1]; }
  inline const T &back() const { return _data[_size - 1]; }
  inline T *data() { return _data; }
  inline const T *data() const { return _data; }

  // Modifiers
  // 完全是模拟构造的过程，甚至保证容量都是一样的

  template <typename InputIterator>
  void assign(InputIterator begin, InputIterator end) {
    /*std:size_t newSize = 0;
    InputIterator it;
    for (it = begin; it != end; ++it)
      ++newSize;*/
    InputIterator it;
    std::size_t newSize = end - begin;

    if (newSize > _capacity) {
      // std::size_t newCapacity = _capacity * 2;  // 此举用意何在？
      std::size_t newCapacity = _capacity;
      while (newSize > newCapacity)
        newCapacity *= 2;
      reserve(newCapacity);  // 重设容量
    }
    std::size_t i;
    /*for (i = 0; i < _size; ++i)
      _a.destroy(_data + i);*/
    this->clear();  // 就是上面那坨
    for (i = 0, it = begin; i < newSize; ++i, ++it)
      _a.construct(_data + i, *it);
    _size = newSize;
  }

  void assign(const std::size_t &newSize, const T &val) {
    if (newSize > _capacity) {
      // std::size_t newCapacity = _capacity * 2;  // 此举用意何在？
      std::size_t newCapacity = _capacity;
      while (newSize > newCapacity)
        newCapacity *= 2;
      reserve(newCapacity);
    }
    std::size_t i;
    /*for (i = 0; i < _size; ++i)
      _a.destroy(_data + i);*/
    this->clear();  // 就是上面那坨
    for (i = 0; i < newSize; ++i)
      _a.construct(_data + i, val);
    _size = newSize;
  }

  // 如果已经满了，就用reserve来扩充容量
  void push_back(const T &val) {
    if (_size >= _capacity)
      reserve(_capacity * 2);
    _a.construct(_data + (_size++), val);
  }

  void pop_back() {
    _a.destroy(_data + (--_size));
  }

  // clear()只析构，不释放内存
  void clear() {
    for (std::size_t i = 0; i < _size; ++i)
      _a.destroy(_data + i);
    _size = 0;
  }

private:
  T *_data;
  std::size_t _size, _capacity;
};

#endif