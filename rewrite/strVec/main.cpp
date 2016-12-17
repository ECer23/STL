#include "queryText.h"
#include "strVec.h"
#include <iostream>
using namespace std;

void runQueries(ifstream& infile) {
  TextQuery tq(infile);
  string word;
  cout << "Please enter the word you want to query" << endl;
  cin >> word;
  cout << tq.query(word);
}

int main() {
  ifstream file("file.txt");
  runQueries(file);
}