# Knowledge about allocator

### `<cstddef>`
`ptrdiff_t` 用于被用于表示两个指针之差。
`size_t` 被用于表示任意一个对象（或者数组）的最大大小。
函数 `size_t offsetof(type, number)` 表示number在数据结构type中距离开始处的位置。  
This macro with functional form returns the offset value in bytes of member member in the data structure or union type type. The value returned is an unsigned integral value of type `size_t` with the number of bytes between the specified member and the beginning of its structure.


### `std::cerr`
`std::cerr`对应标准错误流，用于显示错误消息。默认情况下被关联到标准输出流，但它不被缓冲，也就说错误消息可以直接发送到显示器，而无需等到缓冲区或者新的换行符时，才被显示。一般情况下不被重定向。


### `void*`

void指针可以指向任意类型的数据，就是说可以用任意类型的指针对void指针对void指针赋值。如果要将void指针p赋给其他类型的指针，则需要强制类型转换，就本例而言：`a=（int *）p`

在内存的分配中我们可以见到void指针使用：内存分配函数`malloc()`函数返回的指针就是`void*`型，用户在使用这个指针的时候，要进行强制类型转换，也就是显式说明该指针指向的内存中是存放的什么类型的数据`(int*)malloc(1024)`表示强制规定malloc返回的`void*`指针指向的内存中存放的是一个个的int型数据。


### `new_handler`
new_handler set_new_handler (new_handler new_p) throw();
The new-handler function is a function which is called by the default allocation functions (`operator new` and `operator new[]`) when they fail to allocate storage.

**Return value**  
The value of the current new-handler function if this has already been set by this function previously, or a null-pointer if this is the first call to set_new_handler (or if it was reset by a previous call).


### C++中的new的主要工作如下：
1. 调用malloc函数分配内存空间
2. 如果是new一个对象的话，会调用改对象的构造函数进行初始化操作
3. 返回对象的指针


### `iterator`
`Input Iterator`：只能单步向前迭代元素，不允许修改由该类迭代器引用的元素。
`Output Iterator`：该类迭代器和`Input Iterator`极其相似，也只能单步向前迭代元素，不同的是该类迭代器对元素只有写的权力。
`Forward Iterator`：该类迭代器可以在一个正确的区间中进行读写操作，它拥有Input Iterator的所有特性，和`Output Iterator`的部分特性，以及单步向前迭代元素的能力。
`Bidirectional Iterator`：该类迭代器是在`Forward Iterator`的基础上提供了单步向后迭代元素的能力。
`Random Access Iterator`：该类迭代器能完成上面所有迭代器的工作，它自己独有的特性就是可以像指针那样进行算术计算，而不是仅仅只有单步向前或向后迭代。
这五类迭代器的从属关系，如下图所示，其中箭头A→B表示，A是B的强化类型，这也说明了如果一个算法要求B，那么A也可以应用于其中。
```
  input output  
    \ /  
  forward  
     |  
bidirectional  
     |  
random access  
```


`new (p2) MyClass`  
does not allocate memory -- calls: operator new (sizeof(MyClass),p2)
but constructs an object at p2


### `Iterator traits`
Traits class defining properties of iterators.
member	description
difference_type	Type to express the result of subtracting one iterator from another.
value_type	The type of the element the iterator can point to.
pointer	The type of a pointer to an element the iterator can point to.
reference	The type of a reference to an element the iterator can point to.
iterator_category	The iterator category. It can be one of these:
input_iterator_tag
output_iterator_tag
forward_iterator_tag
bidirectional_iterator_tag
random_access_iterator_tag

<table class="table table-bordered table-condensed">
    <tr>
        <td>**member**</td>
        <td>**description**</td>
    </tr>
    <tr>
        <td>difference_type</td>
        <td>Type to express the result of subtracting one iterator from another.</td>

    </tr>
    <tr>
        <td>value_type</td>
        <td>The type of the element the iterator can point to.</td>
    </tr>
    <tr>
        <td>pointer</td>
        <td>The type of a pointer to an element the iterator can point to.</td>
    </tr>
    <tr>
        <td>reference</td>
        <td>The type of a reference to an element the iterator can point to.</td>

    <tr>
        <td>iterator_category</td>
        <td>The iterator category. It can be one of these:  
        - input_iterator_tag  
        - output_iterator_tag  
        - forward_iterator_tag  
        - bidirectional_iterator_tag  
        - random_access_iterator_tag</td>  
    </tr>


</table>



### `type traits`
在STL中为了提供通用的操作而又不损失效率，我们用到了一种特殊的技巧，叫traits编程技巧。具体的来说，traits就是通过定义一些结构体或类，并利用模板类特化和偏特化的能力，给类型赋予一些特性，这些特性根据类型的不同而异。在程序设计中可以使用这些traits来判断一个类型的一些特性，引发C++的函数重载机制，实现同一种操作因类型不同而异的效果。traits的编程技巧极度弥补了C++语言的不足 。
(还是没有搞太懂...[reference from cplusplus](http://www.cplusplus.com/reference/type_traits/)
