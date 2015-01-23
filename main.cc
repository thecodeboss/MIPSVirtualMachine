#include "machine.h"

int main(int argc, char** argv) {
	assert(argc == 2);
	Machine* machine = new Machine();
	machine->Load(argv[1]);
	machine->Run();
	delete machine;
	return 0;
}