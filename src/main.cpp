#include <iostream>
//#include "Reader.h"

#include "Object.h"

using namespace std;
using namespace octarine;

ObjectFunctions dummyFns = {
  NULL,
  NULL,
  NULL,
  NULL
};

VTable<ObjectFunctions> dummyVTable(NULL, &dummyFns);

Address dummyObj = NULL;

int main(int argc, char** argv) {

  char test[] = "symbol (list of symbols)";

  Object symbol(dummyObj, &dummyVTable);
  Object list(dummyObj, &dummyVTable);

  

  return 0;
}
