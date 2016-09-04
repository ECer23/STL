#ifndef STR_VEC_H
#define STR_VEC_H

#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
using namespace std;

class strVector {
public:
  strVector();
  ~strVector();

  void push_back(const string &);
  void pop_back();

  size_t size() const;
  size_t capacity() const;
  string *begin() const;
  string *end() const;

private:
  void check_alloc();
  void alloc_n_move(size_t new_cap);
  void free();

  /* [ 0 ][ 1 ][ 2 ][ 3 ][  unconstructed   ]
      ^                   ^                 ^
  elements           first_free            cap */
  string *elements;
  string *first_free;
  string *cap;
  allocator<string> alloc;
  /* [ 0 ][ 1 ][ 2 ][ 3 ][   unconstructed   ]
                          ^                  ^
                        old_cap            new_cap */
  void reallocate();
};

#endif // !STR_VEC_H