#ifndef MACHINE_H
#define MACHINE_H

#include "global.h"

class Machine {

	class CPU {
		Machine* machine;
		
		void(CPU::* Ins[0xfff])(uint32);
	  public:
		uint32 PC;
		int32 R[32];
		int32 hi, lo;
		#define c(n) Ins[0x##n]=&CPU::Instruction<0x##n>;
		CPU() : machine(NULL), PC(0), hi(0), lo(0) {
			// R-type
			c(20)c(22)c(18)c(19)
			c(1A)c(1B)c(10)c(12)
			c(14)c(2A)c(2B)c(08)c(09)
			// I-type
			c(8C0)c(AC0)c(100)c(140)
		}
		#undef c
		void Init(Machine *m);
		bool Op();
		
		template<uint32 opcode>
		void Instruction(uint32 op) {
			DEBUG2("Running Instruction: ", std::hex << opcode)
			int32 s, t, i;
			long long temp;
			switch(opcode) {
				case 0x20: // add
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					temp = s + t;
					R[(op >> 11) & 0x1F] = temp;
					break;
				case 0x22: // sub
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					temp = s - t;
					R[(op >> 11) & 0x1F] = temp;
					break;
				case 0x18: // mult
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					temp = s * t;
					hi = temp >> 32;
					lo = temp & 0xFFFFFFFF;
					DEBUG(lo)
					break;
				case 0x19: // multu
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					temp = (uint32)s * (uint32)t;
					hi = temp >> 32;
					lo = temp & 0xFFFFFFFF;
					break;
				case 0x1A: // div
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					hi = s % t;
					lo = s / t;
					break;
				case 0x1B: // divu
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					hi = (uint32)s % (uint32)t;
					lo = (uint32)s / (uint32)t;
					break;
				case 0x10: // mfhi
					R[(op >> 11) & 0x1F] = hi;
					break;
				case 0x12: // mflo
					R[(op >> 11) & 0x1F] = lo;
					break;
				case 0x14: // lis
					R[(op >> 11) & 0x1F] = machine->memory.M[PC++];
					break;
				case 0x2A: // slt
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					R[(op >> 11) & 0x1F] = (s < t) ? 1 : 0;
					break;
				case 0x2B: // sltu
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					R[(op >> 11) & 0x1F] = ((uint32)s < (uint32)t) ? 1 : 0;
					break;
				case 0x08: // jr
					s = R[(op >> 21) & 0x1F];
					PC = s/4;
					break;
				case 0x09: // jalr
					s = R[(op >> 21) & 0x1F];
					R[31] = PC*4;
					PC = s/4;
					break;
				case 0x8C0: // lw
					s = R[(op >> 21) & 0x1F];
					i = op & 0xFFFF;
					R[(op >> 16) & 0x1F] = machine->memory.M[((uint32)s+i)/4];
					break;
				case 0xAC0: // sw
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					i = op & 0xFFFF;
					if ((uint32)s+i == 0xffff000c) std::cout << (char)t;
					machine->memory.M[((uint32)s+i)/4] = t;
					break;
				case 0x100: // beq
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					i = op & 0xFFFF;
					if (s == t) PC += i;
					break;
				case 0x140: // bne
					t = R[(op >> 16) & 0x1F];
					s = R[(op >> 21) & 0x1F];
					i = op & 0xFFFF;
					if (s != t) PC += i;
					break;
			}
			R[0] = 0;
		}
	} cpu;
	
	class Memory {
		Machine* machine;
	  public:
		int32 M[0x3FFFFFFF];
		Memory() : machine(NULL) {}
		void Init(Machine *m);
	} memory;

  public:
	Machine();
	void Load(char *file);
	void Run();
};

#endif