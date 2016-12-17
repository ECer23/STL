# STL_allocator note  

---

## Fundamental
Allocation is implemented by `alloc:allocate`, memory free is implemented  by `alloc:deallocate()`; constructing objects is implemented by `::construct()`, and destructing objects is implemented by `::destory()`.
The whole process is divided into 4 parts:
**memory allocation** ---> **construct objects** --> **destruct objects** --> **free memory**
In header file `<memory>`, there's such declaration.
```c
#include <stl_alloc.h>      // allocate & free
#include <stl_construct.h>  // construction & destroy
```
![1](http://img.my.csdn.net/uploads/201211/29/1354196694_9025.jpg)  
(Pictures below were all from a chinese book《STL源码剖析》by Houjie)
---

## `allocate()` & `deallocate()`
两级配置。第一级配置直接通过`malloc()`实现，而第二级配置是通过 **内存池** 实现。  
内存池  

## `construct()` & `destroy()`
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
