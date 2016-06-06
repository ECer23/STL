#include "Deque.h"
#include <iostream>
#include<exception>
 using namespace std;
 class RuleForbidden: public exception {
  virtual const char *what() const throw() {
    return "Please accept the rules in this assignment.";
  }
};
 int main() {
  // rule check.
  #if (defined(_GLIBCXX_LIST) || defined(_GLIBCXX_VECTOR) ||\
    defined(_GLIBCXX_DEQUE) || defined(_GLIBCXX_STACK))
    throw RuleForbidden();
  #endif
  if (MAP_SIZE != 20 || BUFFER_SIZE != 10) throw RuleForbidden();
   deque<int> que2;
   if (que2.empty()) cout << "The que is empty!" << endl;
  else cout << "The queue is not empty!" << endl;
   // test for iterator.
  int m, n;
  cin >> m >> n;
   for (int i = 0; i < m; i++) que2.push_front(i);
  deque<int>::iterator it = que2.begin();
  while (it != que2.end()) cout << *(it++) << " ";
  cout << endl;
  cout << "The size of deque is: " << que2.size() << endl;
  if (que2.empty()) cout << "The que is empty!" << endl;
  else cout << "The queue is not empty!" << endl;
  que2.pop_front();
  que2.pop_back();
  cout << "The front is: " << que2.front() << endl;
  cout << "The back is: " << que2.back() << endl;
  cout << que2[0] << endl;
  cout << que2[que2.size() - 1] << endl;
  cout << que2[n] << endl;
   cin >> m >> n;
   for (int i = 0; i < m; i++) que2.push_back(i);
  it = que2.begin();
  while (it != que2.end()) cout << *(it++) << " ";
  cout << endl;
  cout << "The size of deque is: " << que2.size() << endl;
  cout << "The front is: " << que2.front() << endl;
  cout << "The back is: " << que2.back() << endl;
  cout << que2[0] << endl;
  cout << que2[que2.size() - 1] << endl;
  cout << que2[n] << endl;
   que2.pop_front();
  que2.pop_front();
  que2.pop_back();
   cout << "Now the queue is: " << endl;
  it = que2.begin();
  while (it != que2.end()) cout << *(it++) << " ";
  cout << endl;
   deque<int>::iterator tmp1 = que2.begin();
  deque<int>::iterator tmp2 = tmp1, tmp3;
  tmp2 = tmp2 + 5;
  cout << *(tmp2++) << endl;
  ++tmp2;
  cout << *(tmp2++) << endl;
  tmp2 = tmp2 - 2;
  cout << *(tmp2++) << endl;
  tmp2 += 25;
  cout << *(tmp2--) << endl;
  --tmp2;
  tmp2 -= 13;
  cout << *tmp2 << endl;
  cout << (tmp1 == que2.begin()) << endl;
  cout << (tmp1 == que2.end()) << endl;
  cout << (tmp1 != tmp2) << endl;
  cout << (tmp1 < tmp2) << endl;
  cout << tmp1[0] << endl;
  cout << tmp1[8] << endl;
  cout << tmp1[23] << endl;
  cout << tmp2[-3] << endl;
   que2.clear();
  cout << "The size of deque is: " << que2.size() << endl;
  m = 13;  // random input m > 20 and m < 30..
  for (int i = 0; i < m; i++) que2.push_front(i);
  it = que2.begin();
  while (it != que2.end()) cout << *(it++) << " ";
  cout << endl;
}
