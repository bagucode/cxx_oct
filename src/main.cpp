#include <iostream>
//#include "Reader.h"

#include "ObjectFunctions.h"
#include "HeapFunctions.h"

using namespace std;
using namespace octarine;

void constructTest(Context* ctx, Object self, Object arg) {
  cout << "construct" << endl;
}


Object copyTest(Context* ctx, Object obj) {
  cout << "copy" << endl;
  return obj; // Should copy the whole graph
}

Object copyToHeapTest(Context* ctx, Heap heap, Object obj) {
  cout << "copyToHeap" << endl;
  return obj;
}

Uword getSizeTest(Context* ctx, Object obj) {
  cout << "getSize" << endl;
  return 100;
}

void traceTest(Context* ctx, Object obj) {
  cout << "trace" << endl;
}

ObjectFunctions dummyFns = {
  constructTest,
  copyTest,
  copyToHeapTest,
  getSizeTest,
  traceTest
};

ObjectVTable dummyVTable = {
  NULL,
  dummyFns
};

Address dummyObj = NULL;

int main(int argc, char** argv) {

  char test[] = "symbol (list of symbols)";

  Object symbol = { dummyObj, &dummyVTable };
  Object list = {dummyObj, &dummyVTable };
  Object nil = {0, 0};

  construct(NULL, symbol, nil);
  Object objCopy = copy(NULL, symbol);
  Heap dummyHeap;
  Object objCopy2 = copyToHeap(NULL, dummyHeap, symbol);
  Uword objCopy3 = getSize(NULL,symbol);
  trace(NULL,symbol);

  return 0;
}
