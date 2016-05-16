# Memory pool
#### Erc23

---
In STL, memory allocation is divided to 2 level. It means there are two versions of allocator. And we call it **sub-allocation**.

### Level 1:  
In level 1, `allocate()` uses `malloc()` directly, and `deallocate()` uses `free()` directly.  

However, a new operation is expensive for it cost a lot of time to traversal the free blocks of the system heap and choose the best blocks for allocation. And sometimes, we allocate memory and free it, then we allocate some memory with nearly the same size again. Then we're wondering if we can use a pool to store those memory so that we don't need allocate from system every time.  

### Level 2:
In level 2, we build a memory pool to store those idle memory. This pool is not so big and if the allocation is huge, we have to allocate from system. When some memory need retrieving, we put it to the memory pool instead of giving it back to system.  
Memory pool is just like your purse. You put some money to your purse so that you don't need to go to bank every time you need money. But if you need much, you have to go to bank.  

---

So the allocation is divided into 3 steps:  
**Step 1**: check if there's enough (128 bytes in standard) memory in memory pool (we get memory in memory pool by `malloc()`).  
**Step 2**: if so, allocate memory from memory pool and finish. if not, go to Step 3.
**Step 3**: use `malloc()` to allocate memory from system.

And the deallocation is divided into 2 steps:  
**Step 1**: put it to the memory pool
**Step 2**: after the whole program done, we `free()` all the memory in memory pool.

---

So how to build a memory pool get the memory in memory pool? I can only tell you how to build and get in STL but it's still difficult for me to build one. (I'm glad if someone could share your idea about it). I'll show a simple version of memory pool at the end of this article.

## How to manage a memory pool (not finished yet)
In STL, a 16-nodes linked list is used to realize the memory pool. Nodes of the list is created through union. It means that if some nodes are used, they will be removed out of the linked list. That's brilliant!
```cpp
union obj {
  union obj* free_list_link;
  char client_data[1];  // the client sees this
};
```
![1](http://img.blog.csdn.net/20150323150345420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzA3NDQ2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)  

## A simple version of memory pool
This is a very simple version of memory pool. It can only allocate/deallocate one block once and those blocks must be the same size. You can view the code in `StackAlloc.hpp` if you want to know more about the limit of this memory manager.  
The core tasks of our memory pool are:  
1. **Use memory pool to allocate**
```cpp
// decide if the memory pool is empty
if (_M_pool == NULL) {
  Node* temp;
  // allocate one block of memory using operator new
  void* p = operator new(sizeof(_Tp));
  // assert this node to memory pool's linked list
  temp = reinterpret_cast<Node* >(p);
  temp->next = _M_pool;
  _M_pool = temp;
}
// if the memory pool is not empty
// convert type Node* to pointer
pointer result = reinterpret_cast<pointer>(_M_pool);
_M_pool = _M_pool->next;
return result;
```

2. **Gather memory when deallocate** (which will be free later)  
```cpp
// gather it directly
if (__p != NULL) {
  // type conversion
  reinterpret_cast<Node *>(__p)->next = _M_pool;
  _M_pool = reinterpret_cast<Node *>(__p);
}
```
