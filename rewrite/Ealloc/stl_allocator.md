# STL_allocator note  

---

## 配置基本原理
内存配置操作由`alloc:allocate`负责， 内存释放由`alloc:deallocate()`负责，对象构造由`::construct()`负责，对象析构由`::destory()`负责。整个内存分配过程分为 从内存池new内存 -> 构造对象；释放过程分为 析构对象 -> 释放内存。
在`<memory>`中有如下声明
```c
#include <stl_alloc.h>      // 内存配置和释放
#include <stl_construct.h>  // 对象构造和析构
```
![1](http://img.my.csdn.net/uploads/201211/29/1354196694_9025.jpg)  

---

## 构造和析构：`construct()`与`destroy()`
destroy有多个版本。
第一个版本接收指针，并析构掉指针所指范围的对象；  
第二个版本接受两个迭代器我们并不知道这个范围有多大，万一范围很大而每个对象的析构函数都是默认析构，那么一次次调用这些没啥用的析构函数对凶啊率有明显下降。所以这里先利用`value_type`获得迭代器所指的对象类型，再用`__type_traits<T>`判断这个类型对应的析构函数是否是默认析构。如果是`true_type`就什么都不做；如果是`false_type`，就循环访问整个范围，并在循环的过程中调用第一个版本的析构。
```js
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type) {
  for ( ; __first != __last; ++__first)
    destroy(&*__first);
}
```
![2](http://img.my.csdn.net/uploads/201211/29/1354196922_8237.jpg)
