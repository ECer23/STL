# Introduction
A text query program, using a string vector, a simplification of the library vector class. So it's just for practicing, with a little practical significance.   
This program has two parts: `strVector` and `TextQuery`. 
You can edit `file.txt` and query some word , just have a try!

# strVector
## Class design
Class `strVector` preallocates enough storage named `capacity()` to hold more elements than needed. And those storgae will be constructed until we need them. Constructed storage is named `size()`.  
Each action to add element checks if there is enough space available in `capacity()`. If not, we need to reallocate more storgae(2 times than before) and move elements to new strogae.   
To obtain acceptable performance, we need an allocator to get raw memeory, construct/destory them, and free them at last.  
```
[ 0 ][ 1 ][ 2 ][ 3 ][  unconstructed   ]
  ^                   ^                ^
elements           first_free         cap
```
Each StrVec will have three pointers into the space it uses for its elements:
- elements, which points to the first element in the allocated memory
- first_free, which points just after the last actual element
- cap, which points just past the end of the allocated memory  

## Memory manage
### constructor and destructor
When a `strVector` is constructed first time, `elements`, `first_free` and `cap` are all set to `nullptr`. And when destructed, we call member `free()`.  
Member `free()` need to destory element and deallocate memory.   
We should check if element is nullptr, which means the vector is empty.    
For convenience, destory begins at `first_free`. And because block that `first_free` points to is unconstructed, we need to move it forward before destorying.
```
[ 1 ][ 2 ][  ][  ]
      ^    ^
     --p   p
```
```cpp
void strVector::free() {
  if (elements != nullptr) {
    for (auto p = first_free; p != elements; /* empty */)
      alloc.destroy(--p);
    alloc.deallocate(elements, capacity());
  }
}
```

### element access
Recall vector in standard library, we can use an iterator to access element from both sides. `begin()` points to the first element, `end()` points to next block of the last element.  
Add and remove element are restricted from back. Take `push_back()` as an example, we need to check if there is enough memeory. If so, we construct `first_free` and move it to next block.  
```cpp
void strVector::push_back(const string& s) {
  check_alloc();
  alloc.construct(first_free++, s);
}
```

### element move
In member `reallocate()`, we need move element from old `strVector` to new `strVector`. But copying the data in these strings is unnecessary. Our StrVec’s performance will be much better if we can avoid the overhead of allocating and deallocating the strings themselves each time we reallocate.  
To avoid copying, we use [`std::move`](http://www.cplusplus.com/reference/utility/move/?kw=move), which is defined in `<utility>`  

# Text Query
## Class design
### Targets
1. When it reads the input, it should remember every line that every word appears. 
2. When it generates output,
- The program must be able to fetch the line numbers associated with a given
word
- The line numbers must appear in ascending order with no duplicates
- The program must be able to print the text appearing in the input file at a given line number.

### Data structure
- We use `strVector` to store every line of the text file and `std::isstringstream` to desperate word.
- We use `std::set` to store the line number, with which we don't need to sort the line number.
- We use `std::map` to associate a word with its line number(`std::set`).

### Class declaration
To query word, we define a class `TextQuery`. It obtains a `std::map` and a `std::vector`. So searching finishes after the class being constructed.    
And we define another class `QueryResult` to print search answer. It overrides the operator `<<`. 

## Details of definition
### share_ptr
`QueryResult` need to express the searching answer, which is saved in `TextQuery`. If we copy a duplicate, it wastes time and space, which may cause a copy of the whole file. And it's not safe to return a pointer/iterator to another class, so `share_ptr` couldn't be a better choice. Auto pointers in both class pointed to the same storage, and it'll auomatically free memory.
```cpp
class TextQuery {
/* other members */
  map<string, shared_ptr<set<line_no>>> file_map;
  shared_ptr<strVector> fptr_vec;
};

class QueryResult {
/* other members */
  shared_ptr<strVector> fptr_vec;
  shared_ptr<set<line_no>> lines;
```