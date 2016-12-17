#ifndef DEQUEITERATOR_H_
#define DEQUEITERATOR_H_
#include <exception>
#include <iostream>
using namespace std;
#define BUFFER_SIZE 10
template <class T, class Ref = T &, class Ptr = T *>  // Bufsize表示缓冲区的大小
struct DequeIterator {
  //定义迭代器的类型
  typedef DequeIterator<T> iterator;
  typedef DequeIterator<T> const_iterator;
  typedef random_access_iterator_tag iterator_category;
  //返回缓冲区的大小，以个为单位
  static size_t buffer_size() { return BUFFER_SIZE; }

  /***********************类型别名***********************/
  typedef T value_type;               //元素类型
  typedef Ptr pointer;                //指针类型
  typedef Ref reference;              //引用类型
  typedef size_t size_type;           //大小类型
  typedef ptrdiff_t difference_type;  //指针差值类型
  typedef T **map_pointer;  //想到二维数组的数组名就是一个二级指针
  map_pointer node;         // node可以看成是第一维

  /***************deque迭代器包含的几种重要的属性***************/
  T *first;  //缓冲区开始处，可看成是第二维第一个元素的位置
  T *last;  //缓冲区末端的下一个位置，第二维的最后元素的下一个位置
  T *curr;  //可以看成是原生指针，表示当前的元素位置
  //如果是第一个块缓冲区，则指向当前实际的元素位置；
  //如果是最后一个缓冲区，则指向当前实际元素的下一个位置，符合STL中[...)的原则

  /**********************Constructors**********************/
  // y's value is the address of firstbuffer's address.
  DequeIterator(T *x, map_pointer y) {
    node = y;
    first = *y;
    curr = x;
    last = *y + BUFFER_SIZE;
  }

  explicit DequeIterator(map_pointer y) {
    node = y;
    first = *y;
    curr = *y;
    last = *y + BUFFER_SIZE;
  }

  DequeIterator() {
    node = NULL;
    first = NULL;
    curr = NULL;
    last = NULL;
  }

  /************迭代器的操作*************/
  // return *curr;定义解引用操作
  reference operator*() const { return *curr; }
  // return current;定义箭头操作符
  reference operator->() const { return curr; }
  //定义两迭代器相减的操作
  difference_type operator-(const iterator &x) const {
    iterator temp = (*this);
    int count = 0;
    while (temp != x) {
      temp--;
      count++;
    }
    return difference_type(count);
    // return difference_type(buffer_size()) * (node - x.node - 1) +
    //      (curr - first) + (x.last - x.curr);
  }
  typedef iterator self;
  bool operator==(const self &x) const { return curr == x.curr; }
  bool operator!=(const self &x) const { return !(curr == x.curr); }
  //先比较第一维，相同则再比较第二维
  bool operator<(const self &x) const {
    return (node == x.node) ? (curr < x.curr) : (node < x.node);
  }

  //将当前的迭代器设置为new_node,主要是设置node、first、last属性的值
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }

  //定义前置自加,++p，返回引用,因为是return *this
  self &operator++() {
    curr = curr + 1;
    if (curr == last) {
      set_node(node + 1);
      curr = first;
    }
    return (*this);
  }

  //定义后置自加,p++，返回非引用，因为是return temp,不能返回局部对象的引用
  self operator++(int) {
    iterator temp = (*this);
    ++(*this);
    return temp;
  }

  //定义前置自减，--p，返回引用
  self &operator--() {
    if (curr == first) {
      set_node(node - 1);
      curr = last;
    }
    curr = curr - 1;
    return (*this);
  }

  //定义后置自减，p--，返回非引用
  self operator--(int) {
    iterator temp = (*this);
    --(*this);
    return temp;
  }

  //将本迭代器自加n步，随机访问迭代器的属性
  self &operator+=(difference_type n) {
    while (n--) {
      ++(*this);
    }
    return (*this);
  }

  //本迭代器自减n步，随机访问迭代器的属性
  self &operator-=(difference_type n) {
    while (n--) {
      --(*this);
    }
    return (*this);
  }

  //返回一个将本迭代器减n后的temp值，随机访问迭代器的属性
  self operator-(difference_type n) {
    iterator temp = (*this);
    if (n < 0) {
      temp -= (-n);
    } else {
      temp += n;
    }
    return temp;
  }

  self operator+(difference_type n) {
    iterator temp = (*this);
    if (n > 0) {
      temp += n;
    } else {
      temp -= (-n);
    }
    return temp;
  }

  //定义下标访问功能，随机访问迭代器的属性
  reference operator[](difference_type n) const {
    return *((iterator)(*this) + n);
  }
};
#endif
