#ifndef STR_VEC_H
#define STR_VEC_H

#include <string>
#include <memory>
#include <exception>
using namespace std;

class strVector {
 public:
  strVector();
  strVector(strVector &) noexcept;
  strVector(strVector &&) noexcept;
  ~strVector();

  void push_back(const string &);
  void pop_back();

  size_t size() const;
  size_t capacity() const;
  string *begin() const;
  string *end() const;

  strVector &operator=(const strVector &);
  strVector &operator=(strVector &&) noexcept;

 private:
  // check if strVector::reallocate() is needed
  void check_alloc();
  // move (*this) to a new bigger space
  void alloc_n_move(size_t);
  // tool function, used by copy constructor, assign operator
  pair<string *, string *> alloc_n_copy(const string *, const string *);
  // destory() and deallocate()
  void free();

  allocator<string> alloc;

  /* [ 0 ][ 1 ][ 2 ][ 3 ][  unconstructed   ]
      ^                   ^                 ^
  elements           first_free            cap */
  string *elements, *first_free, *cap;

  /* [ 0 ][ 1 ][ 2 ][ 3 ][   unconstructed   ]
                          ^                  ^
                        old_cap            new_cap */
  // allocate more memeory when needed
  void reallocate();
};

#endif  // !STR_VEC_H