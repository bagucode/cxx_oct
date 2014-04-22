#include <iostream>
//#include "Reader.h"

#include "Object.h"

using namespace std;
using namespace octarine;

Object copyTest(Object obj) {
  cout << "copy" << endl;
  return obj; // Should copy the whole graph
}

Object copyToHeapTest(Object obj) {
  cout << "copyToHeap" << endl;
  return obj;
}

Uword getSizeTest(Object obj) {
  cout << "getSize" << endl;
  return 100;
}

void traceTest(Object obj) {
  cout << "trace" << endl;
}

ObjectFunctions dummyFns = {
  copyTest,
  copyToHeapTest,
  getSizeTest,
  traceTest
};

VTable<ObjectFunctions> dummyVTable = {
  NULL,
  dummyFns
};

Address dummyObj = NULL;

int main(int argc, char** argv) {

  char test[] = "symbol (list of symbols)";

  Object symbol = { dummyObj, &dummyVTable };
  Object list = {dummyObj, &dummyVTable };

  Object objCopy = copy(symbol);
  Object objCopy2 = copyToHeap(symbol);
  Uword objCopy3 = getSize(symbol);
  trace(symbol);

  return 0;
}
