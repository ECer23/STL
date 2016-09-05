#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <memory>
#include <exception>
using namespace std;

class String {
friend ostream& operator << (ostream&, String s);
 public:
  String();
  String(const char *);
  String(String &) noexcept;
  String(String &&) noexcept;
  ~String();

  void push_back(const char &);
  void pop_back();

  size_t size() const;
  void resize(size_t, const char & = char());

  size_t capacity() const;
  void reserve(size_t);

  char *begin() const;
  char *end() const;

  String operator+(const String &);
  String &operator+=(const String &);
  String &operator=(const String &);
  String &operator=(String &&) noexcept;

 private:
  // check if Stri id check_alloc();
  void check_alloc();
  // move (*this) to a new bigger space
  void alloc_n_move(size_t);
  // tool function, used by copy constructor, assign operator
  pair<char *, char *> alloc_n_copy(const char *, const char *);
  // destory() and deallocate()
  void free();

  allocator<char> alloc;

  /* [ 0 ][ 1 ][ 2 ][ 3 ][  unconstructed   ]
      ^                   ^                 ^
  elements           first_free            cap */
  char *elements, *first_free, *cap;

  /* [ 0 ][ 1 ][ 2 ][ 3 ][   unconstructed   ]
                          ^                  ^
                        old_cap            new_cap */
  // allocate more memeory when needed
  void reallocate();
};

#endif  // !STRING_H