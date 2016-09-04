#include "strVec.h"
#include <utility>  // std::move

strVector::strVector() { elements = first_free = cap = nullptr; }

strVector::~strVector() { free(); }

void strVector::push_back(const string& s) {
  check_alloc();
  alloc.construct(first_free++, s);
}

void strVector::pop_back() {
  if (size() == 0) {
    throw out_of_range("pop on empty strVec");
  } else {
    alloc.destroy(first_free--);
  }
}

size_t strVector::size() const { return size_t(first_free - elements); }
size_t strVector::capacity() const { return size_t(cap - elements); }
string* strVector::begin() const { return elements; }
string* strVector::end() const { return first_free; }

void strVector::check_alloc() {
  if (this->size() == this->capacity()) reallocate();
}

void strVector::reallocate() {
  auto newcapacity = size() ? 2 * size() : 1;
  alloc_n_move(newcapacity);
}

void strVector::alloc_n_move(size_t new_cap) {
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

void strVector::free() {
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