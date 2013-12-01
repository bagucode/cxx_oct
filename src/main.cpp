#include "Runtime.h"

using namespace octarine;

int main(int argc, char* argv[]) {
    
    Runtime rt = createRuntime();
    
    destroyRuntime(rt);
    
    return 0;
}
