#include "String.h"
#include <string.h>  // strlen
#include <utility>   // std::move

ostream& operator<<(ostream& os, const String s) {
  for (auto e : s) os << e;
  return os;
}

String::String() { elements = first_free = cap = nullptr; }

String::String(const char* rhs) {
  auto data = alloc_n_copy(rhs, rhs + strlen(rhs));
  elements = data.first;
  first_free = cap = data.second;
}

String::String(String& rhs) noexcept {
  auto data = alloc_n_copy(rhs.begin(), rhs.end());
  elements = data.first;
  first_free = cap = data.second;
}

String::String(String&& s) noexcept : elements(s.elements),
                                      first_free(s.first_free),
                                      cap(s.cap) {
  s.elements = s.first_free = s.cap = nullptr;
}

String::~String() { free(); }

String String::operator+(const String& s) {
  String temp = *this;
  for (auto e : s) temp.push_back(e);
  return temp;
}

String& String::operator+=(const String& s) {
  *this = *this + s;
  return *this;
}

String& String::operator=(const String& rhs) {
  if (this != &rhs) {
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    elements = data.first;
    first_free = cap = data.second;
  }
  return *this;
}

String& String::operator=(String&& rhs) noexcept {
  if (this != &rhs) {
    free();
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;
    rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
  return *this;
}

void String::push_back(const char& s) {
  check_alloc();
  alloc.construct(first_free++, s);
}

void String::pop_back() {
  if (size() == 0) {
    throw out_of_range("pop on empty strVec");
  } else {
    alloc.destroy(first_free--);
  }
}

size_t String::size() const { return size_t(first_free - elements); }

void String::resize(size_t n, const char& val) {
  if (n > size()) {
    if (capacity() < n) reserve(n);
    auto t = n - size();
    while (t--) alloc.construct(first_free++, val);
  }
}

size_t String::capacity() const { return size_t(cap - elements); }

void String::reserve(size_t n) {
  if (capacity() < n) {
    auto newcapacity = n;
    alloc_n_move(newcapacity);
  }
}

char* String::begin() const { return elements; }
char* String::end() const { return first_free; }

void String::check_alloc() {
  if (this->size() == this->capacity()) reallocate();
}

void String::reallocate() {
  auto newcapacity = size() ? 2 * size() : 1;
  alloc_n_move(newcapacity);
}

void String::alloc_n_move(size_t new_cap) {
  auto newdata = alloc.allocate(new_cap);
  auto elem = elements;
  auto dest = newdata;
  // use std::move() rather than move()
  for (auto i = 0; i != size(); i++) {
    alloc.construct(dest++, std::move(*elem++));
  }
  free();
  first_free = dest;
  elements = newdata;
  cap = elements + new_cap;
}

pair<char*, char*> String::alloc_n_copy(const char* b, const char* e) {
  auto data = alloc.allocate(e - b);
  return {data, uninitialized_copy(b, e, data)};
}

void String::free() {
  if (elements != nullptr) {
    // first_free is pointing to the first unconstructed string block
    // [ 1 ][ 2 ][  ][  ]
    //        ^    ^
    //       --p   p
    for (auto p = first_free; p != elements; /* empty */) {
      alloc.destroy(--p);
    }
    alloc.deallocate(elements, capacity());
  }
}