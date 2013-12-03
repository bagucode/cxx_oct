#include "octarine.cpp"

using namespace octarine;

int main(int argc, char* argv[]) {
    
    Runtime rt = createRuntime();
	ThreadContext tc = getThreadContext();

	eval(tc, createString(tc, "(println \"Hello World!\")"));
    
    destroyRuntime(rt);
    
    return 0;
}
