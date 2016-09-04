#include "queryText.h"
#include <iostream>
using namespace std;

TextQuery::TextQuery(ifstream& infile) : fptr_vec(new strVector) {
  string line;
  int count = 1;
  while (getline(infile, line)) {
    fptr_vec->push_back(line);
    istringstream text(line);
    string word;
    while (text >> word) {
      auto& lines = file_map[word];
      if (!lines) lines.reset(new set<line_no>);
      lines->insert(count);
    }
    count++;
  }
}

QueryResult TextQuery::query(const string& word) const {
  static shared_ptr<set<line_no>> nodata(new set<line_no>);
  auto it = file_map.find(word);
  if (it == file_map.end()) {
    return QueryResult(word, nodata, fptr_vec);
  } else {
    return QueryResult(word, it->second, fptr_vec);
  }
}

QueryResult::QueryResult(string s, shared_ptr<set<line_no>> fl,
                         shared_ptr<strVector> fv)
    : word(s), fptr_vec(fv), lines(fl) {}

ostream& operator<<(ostream& os, const QueryResult::QueryResult qr) {
  os << qr.word << " occurs " << qr.lines->size() << " "
     << "times" << endl;
  for (auto num : *qr.lines) {
    os << "  "
       << "(line " << num << ")" << *(qr.fptr_vec->begin() + num - 1) << endl;
  }
  return os;
}
