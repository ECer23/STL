#include <iostream>
#include <cassert>
#include <time.h>
#include <vector>
#include <vld.h>

#include "MemoryManager.hpp"
#include "StackAlloc.hpp"
 
/* Adjust these values depending on how much you trust your computer */
// #define ELEMS 520000
#define REPS 50
 
int main() {
  clock_t start;
  int ELEMS;
  std::cin >> ELEMS;
  /* use the default allocator */
  std::cout << "/* use the default allocator */" << std::endl;
   StackAlloc<int, std::allocator<int> > stackdefault;
   start = clock();
   for (int j = 0; j < REPS; j++) {
     assert(stackdefault.empty());
     for (int i = 0; i < ELEMS / 4; i++) {
       // unroll to time the actual code and not the loop
       stackdefault.push(i);
       stackdefault.push(i);
       stackdefault.push(i);
       stackdefault.push(i);
     }
     for (int i = 0; i < ELEMS / 4; i++) {
       // unroll to time the actual code and not the loop
       stackdefault.pop();
       stackdefault.pop();
       stackdefault.pop();
       stackdefault.pop();
     }
   }
   std::cout << "default allocator time: ";
   std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
 
  /* Use MemoryManager */
   std::cout << "/* Use MemoryManager */" << std::endl;
  StackAlloc<int, MemoryManager<int> > stackPool;
  start = clock();
  for (int j = 0; j < REPS; j++) {
    assert(stackPool.empty());
    for (int i = 0; i < ELEMS / 4; i++) {
      // Unroll to time the actual code and not the loop
      stackPool.push(i);
      stackPool.push(i);
      stackPool.push(i);
      stackPool.push(i);
    }
    for (int i = 0; i < ELEMS / 4; i++) {
      // Unroll to time the actual code and not the loop
      stackPool.pop();
      stackPool.pop();
      stackPool.pop();
      stackPool.pop();
    }
  }
 
  std::cout << "Done Processing allocation." << std::endl;
  std::cout << "You can run the program locally to check the difference "
               "between MemoryManager and std::Alloc"
            << std::endl;
  std::cout << "MemoryManager Allocator Time: ";
  std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
 
  return 0;
}
 
 