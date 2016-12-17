#include <iostream>
#include "AVLtree.h"
using namespace std;

// Test Main
int main() {
  AVLtree<int>* tree = new AVLtree<int>();
  cout << "First, Test insert(key) funciton: " << endl;
  cout << "Test LL : " << endl;
  // test LL
  tree->insert(8);
  tree->insert(6);
  tree->insert(11);
  tree->insert(4);
  tree->insert(7);
  tree->insert(2);
  cout << "DLR Output LL: " << endl;
  tree->getRoot()->output();
  tree->clear();

  // test RR
  cout << endl << endl << "Test RR : " << endl;
  tree->insert(8);
  tree->insert(6);
  tree->insert(10);
  tree->insert(9);
  tree->insert(12);
  tree->insert(14);
  cout << "DLR Output RR: " << endl;
  tree->getRoot()->output();
  tree->clear();

  // test LR
  cout << endl << endl << "Test LR : " << endl;
  tree->insert(9);
  tree->insert(6);
  tree->insert(11);
  tree->insert(4);
  tree->insert(7);
  tree->insert(8);
  cout << "DLR Output LR: " << endl;
  tree->getRoot()->output();
  tree->clear();

  // test RL
  cout << endl << endl << "Test RL : " << endl;
  tree->insert(6);
  tree->insert(4);
  tree->insert(12);
  tree->insert(10);
  tree->insert(14);
  tree->insert(8);
  cout << "DLR Output RL: " << endl;
  tree->getRoot()->output();
  tree->clear();

  // test erase(const Type& )
  cout << endl << endl << "Test erase : " << endl;
  tree->insert(6);
  tree->insert(7);
  tree->insert(9);
  tree->insert(13);
  tree->insert(15);
  tree->insert(4);
  tree->insert(5);
  tree->insert(17);
  tree->insert(19);
  tree->insert(12);
  tree->insert(10);
  tree->insert(14);
  tree->insert(8);
  cout << "DLR Output Before erase: " << endl;
  tree->output();
  tree->erase(7);
  cout << endl << "DLR Output After erase: " << endl;
  tree->output();
  tree->clear();

  return 0;
}