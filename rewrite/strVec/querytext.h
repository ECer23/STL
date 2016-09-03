#ifndef QUERY_TEXT_H
#define QUERY_TEXT_H

#include "strVec.h"
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
using namespace std;

using line_no = size_t;

class QueryResult;
class TextQuery {
public:
  TextQuery(ifstream &);
  QueryResult query(const string &) const;

private:
  map<string, shared_ptr<set<line_no>>> file_map;
  shared_ptr<strVector> fptr_vec;
};

class QueryResult {
public:
  QueryResult(string, shared_ptr<set<line_no>>, shared_ptr<strVector>);

  friend ostream &operator<<(ostream &, const QueryResult);

private:
  string word;
  shared_ptr<strVector> fptr_vec;
  shared_ptr<set<line_no>> lines;
};

#endif // QUERY_TEXT_H