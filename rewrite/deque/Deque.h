#ifndef DEQUE_H_
#define DEQUE_H_
#include "DequeIterator.h"
#define MAP_SIZE 20

template <class T>
class deque {
 public:
  /***********定义公有访问属性****************/
  typedef T value_type;               // 元素类型
  typedef value_type *pointer;        // 指针类型
  typedef T &reference;               // 引用类型
  typedef ptrdiff_t difference_type;  // 迭代器差值类型
  typedef size_t size_type;           // 大小类型
  typedef DequeIterator<T> iterator;  // 迭代器类型

  /***********构造函数/析构函数*****************/
  deque() {
    map_size = MAP_SIZE;
    map = new pointer[map_size];
    for (int i = 0; i < map_size; i++) {
      map[i] = new value_type[BUFFER_SIZE];
      for (int j = 0; j < BUFFER_SIZE; j++) {
        map[i][j] = 0;
      }
    }
    finish = start = iterator(map + 10);
  }

  ~deque() {
    for (int i = 0; i < map_size; i++) {
      delete[] map[i];
      map[i] = NULL;
    }
    delete[] map;
    map = NULL;
  }

  /*************插入操作*********************/
  void push_back(const T &value) {
    *finish = value;
    finish++;
  }
  void push_front(const T &value) {
    start--;
    *start = value;
  }

  /*************删除操作********************/
  void clear() { finish = start = iterator(map + 10); }
  void pop_back() { finish--; }
  void pop_front() { start++; }

  /*************大小操作*******************/
  //返回容器内元素的格式
  size_type size() const {
    return static_cast<difference_type>(finish - start);
  }
  //判断容器是否为空
  bool empty() const { return (start == finish); }

  /*************访问操作*****************/

  //返回头指针
  iterator begin() const { return start; }
  //返回末端元素的下一个位置
  iterator end() const { return finish; }
  //返回第一个元素的引用
  reference front() const { return (*start); }
  //返回最后一个元素的引用
  reference back() {
    iterator temp = finish;
    temp--;
    return (*temp);
  }
  //返回第i个元素的引用
  reference operator[](size_type i) { return this->at(i); }
  //返回第i个元素的引用
  reference at(size_type i) { return start[static_cast<difference_type>(i)]; }

 protected:
  typedef T **map_pointer;  //定义指向map类型（相当于第一维）的类型别名
  iterator start;      //指向map（第一维）的第一个元素
  iterator finish;     //指向map（第二维）的最后一个有效的元素
  map_pointer map;     //相当于二维数组的首地址
  size_type map_size;  //定义第一维的元素个数
};

#endif
