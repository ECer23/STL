#include <iostream>
#include <time.h>
#include <vector>
#include "myVector.h"

void Print(myVector<int>& v) {
  for (auto e : v) {
    cout << e << " ";
  }
  cout << endl;
}

void Print(vector<int>& v) {
  for (auto e : v) {
    cout << e << " ";
  }
  cout << endl;
}

void test1() {
  clock_t start, finish;
  double totaltime;
  start = clock();

  myVector<int> v;
  for (auto i = 0; i < 9999999; i++) v.push_back(i + 1);

  finish = clock();
  totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
  cout << "\nrun time of this procedure is " << totaltime << "second！" << endl
       << endl;
}

void test2() {
  clock_t start, finish;
  double totaltime;
  start = clock();

  vector<int> v;
  for (auto i = 0; i < 9999999; i++) v.push_back(i + 1);

  finish = clock();
  totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
  cout << "\nrun time of this procedure is " << totaltime << "second！" << endl
       << endl;
}

int main() {
  test2();
  test1();
}
