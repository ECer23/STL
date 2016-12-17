#include "String.h"
#include <iostream>
using namespace std;

int main() {
  /*** check 1 ***/
  String a;
  String b("haha");

  cout << "a.size(): " << a.size() << endl;
  cout << "a.capacity(): " << a.capacity() << endl;
  cout << "b.size(): " << b.size() << endl;
  cout << "b.capacity(): " << b.capacity() << endl;

  a.push_back('a');
  a.push_back('b');
  a.reserve(8);

  cout << "a.size(): " << a.size() << endl;
  cout << "a.capacity(): " << a.capacity() << endl;

  b.pop_back();
  cout << "b.size(): " << b.size() << endl;
  cout << "b.capacity(): " << b.capacity() << endl;

  b.resize(8);
  cout << "b.size(): " << b.size() << endl;
  cout << "b.capacity(): " << b.capacity() << endl;

  /*** check 2 ***/
  String c("My name ");
  String d("is ");
  d += "Apple";
  String e = c + d;

  cout << c << endl;
  cout << d << endl;
  cout << e << endl;

  return 0;
}