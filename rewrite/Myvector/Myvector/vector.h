#ifndef __VECTOR_H__
#define __VECTOR_H__

#define _a Alloc()
// allocate(size_type n) ������ôn��value_type���ڴ�
// construct(pointer p, value_type value) ����ָ��λ�õ��ڴ�Ϊֵvalue
// deallocate(pointer p, std::size_type n) ֻ�ͷŶ�������ڴ棬n�����С������ϵͳ�����ӽ��ڴ�أ�
// destroy(pointer p) ֻ�ͷ���pָ��Ķ�������Ķ�̬�ڴ�

#include "base.h"
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class myVector : public Base {
public:
  // Constructor
  // ����˼·��allocate�����ڴ棬��size��capacity����Ϊͬ����С��Ȼ����й��죨��ֵ��
  // ����ֻ���ڴ棬������

  // Ĭ�Ϲ������������Ϊ1����С����Ϊ0������һ���ڴ棬���������и�ֵ
  myVector() {
    _data = _a.allocate(1);  // ��Ĭ�ϵ�allocator�����ڴ�
    _size = 0;
    _capacity = 1;
  }

  // a�ǿ�ѡ��allocater
  myVector(const std::size_t &size, const T &val, Alloc a = Alloc()) {
    _data = a.allocate(size);  // ����size��С���ڴ�
    for (std::size_t i = 0; i < size; ++i)
      a.construct(_data + i, val);  // ����
    _size = _capacity = size;  // ����ҲΪsize��С
  }

  template <typename InputIterator>
  myVector(InputIterator begin, InputIterator end, Alloc a = Alloc()) {
    _size = _capacity = end - begin;  // ����ǵ�������ָ�룩���
    _data = a.allocate(_size);
    std::size_t cnt = 0;
    for (InputIterator it = begin; it != end; ++it)
      a.construct(_data + (cnt++), *it);
  }

  myVector(const myVector &other) {
    _size = other._size;
    _capacity = other._capacity;
    _data = _a.allocate(_capacity);  // ������ڴ�����������С��ͬ
    for (std::size_t i = 0; i < _size; ++i)  // ���ǹ����ʱ�򲻹�����������
      _a.construct(_data + i, other._data[i]);
  }

  // Destructor
  // ����˼·���ȴݻ٣������������ͷ��ڴ棨һ���ԣ�
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

  // reserveֻ�ı������������������ı�������С
  // ���������С���ͰѶ��ɾ���ͺ�
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
  // ��ȫ��ģ�⹹��Ĺ��̣�������֤��������һ����

  template <typename InputIterator>
  void assign(InputIterator begin, InputIterator end) {
    /*std:size_t newSize = 0;
    InputIterator it;
    for (it = begin; it != end; ++it)
      ++newSize;*/
    InputIterator it;
    std::size_t newSize = end - begin;

    if (newSize > _capacity) {
      // std::size_t newCapacity = _capacity * 2;  // �˾�������ڣ�
      std::size_t newCapacity = _capacity;
      while (newSize > newCapacity)
        newCapacity *= 2;
      reserve(newCapacity);  // ��������
    }
    std::size_t i;
    /*for (i = 0; i < _size; ++i)
      _a.destroy(_data + i);*/
    this->clear();  // ������������
    for (i = 0, it = begin; i < newSize; ++i, ++it)
      _a.construct(_data + i, *it);
    _size = newSize;
  }

  void assign(const std::size_t &newSize, const T &val) {
    if (newSize > _capacity) {
      // std::size_t newCapacity = _capacity * 2;  // �˾�������ڣ�
      std::size_t newCapacity = _capacity;
      while (newSize > newCapacity)
        newCapacity *= 2;
      reserve(newCapacity);
    }
    std::size_t i;
    /*for (i = 0; i < _size; ++i)
      _a.destroy(_data + i);*/
    this->clear();  // ������������
    for (i = 0; i < newSize; ++i)
      _a.construct(_data + i, val);
    _size = newSize;
  }

  // ����Ѿ����ˣ�����reserve����������
  void push_back(const T &val) {
    if (_size >= _capacity)
      reserve(_capacity * 2);
    _a.construct(_data + (_size++), val);
  }

  void pop_back() {
    _a.destroy(_data + (--_size));
  }

  // clear()ֻ���������ͷ��ڴ�
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