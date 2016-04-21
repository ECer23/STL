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


![2](http://img.my.csdn.net/uploads/201211/29/1354196922_8237.jpg)
