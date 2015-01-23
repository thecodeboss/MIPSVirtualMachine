#include "machine.h"
using namespace std;

//-------------- Helpers -------------
uint32 ReadCorrect (uint32 i) {
	unsigned char b1, b2, b3, b4;
	b1 = i & 255;
	b2 = ( i>> 8 ) & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;
	return ((uint32)b1 << 24) + ((uint32)b2 << 16) + ((uint32)b3 << 8) + b4;
}

//------------- Machine ------------

Machine::Machine() {}

void Machine::Load(char *file) {
	DEBUG2("Loading file: ", file)
	cpu.Init(this);
	memory.Init(this);

	ifstream f(file, ios::in|ios::binary|ios::ate);
	if (!f.is_open()) {
		DEBUG2("Failed to open file: ", file)
		return;
	}
	ifstream::pos_type size = f.tellg();
	f.seekg (0, ios::beg);
	f.read ((char*)&memory.M, size);
	f.close();
	DEBUG2("Finished loading file: ", file)
	
	for (uint32 i=0; i<size; i+=4) memory.M[i/4] = ReadCorrect(memory.M[i/4]);
}

void Machine::Run() {
	while (cpu.Op()) {
		// Handle input?
	}
}

//-------------- CPU -----------------

void Machine::CPU::Init(Machine* m) {
	machine = m;
	
	for (int i=0; i<32; i++) R[i] = 0;
	R[1] = -2147483648;
	R[30] = 40000;
	R[31] = 40001;
}

bool Machine::CPU::Op() {
	uint32 op = (uint32)machine->memory.M[PC++];
	if (op == 0) {
		DEBUG("Program completed successfully!")
		return false;
	}
	uint32 opcode = op & 0xFC000000;
	if (opcode == 0) opcode = op & 0x3F;
	else opcode >>= 20;
	if (Ins[opcode] == NULL) {
		PANIC2("Invalid instruction: ", std::hex << opcode)
		return false;
	}
	(this->*Ins[opcode])(op);
	return true;
}

//-------------- Memory -----------------

void Machine::Memory::Init(Machine* m) {
	machine = m;
}
