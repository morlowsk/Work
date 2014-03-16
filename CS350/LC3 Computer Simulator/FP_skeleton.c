// *** Note: This file doesn't compile as is ***

// CS 350, Final Project, Fall 2013 (skeleton file)
//
// LC3 simulator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Basic Declarations
//
	typedef short int Word;
	typedef unsigned short int Address;

	#define MEMLEN 65536
	#define NBR_REGS 8

	typedef struct {
		Word memory[MEMLEN];
		Word reg[NBR_REGS];  // Note: "register" is a reserved word
		int condition_code;  // positive, zero, or negative
		Word instr_reg;
		Address pgm_counter;
		int running;         // running = 1 iff CPU is executing instructions
	} CPU;


// Prototypes
//
	// CPU manipulation and status dump functions
	//
	void init_CPU(CPU *cpu);   // Simulate power-up
	void dump_CPU(CPU *cpu);
	void dump_control_unit(CPU *cpu);
	void dump_memory(CPU *cpu, Address from, Address to);
	char get_condition_code(CPU *cpu);
	void set_condition_code(CPU *cpu, Word value);

	// Instruction cycle functions
	//
	void instruction_cycle(CPU *cpu);      // Simulate entire instruction cycle
	void fetch_instr(CPU *cpu);            // Simulate fetch instruction part of cycle
	// *** STUB *** You may want other routines (you may not)

	// Functions for accessing parts of an instruction
	//
	int op(Word w);                       // Opcode of an instruction
	// *** STUB *** You probably want other routines

	// Functions for executing each instruction
	//
	void instr_ADD (CPU *cpu);
	void instr_AND (CPU *cpu);
	void instr_BR  (CPU *cpu);
	void instr_err (CPU *cpu);
	void instr_JMP (CPU *cpu);
	void instr_JSR (CPU *cpu);
	void instr_LD  (CPU *cpu);
	void instr_LDI (CPU *cpu);
	void instr_LDR (CPU *cpu);
	void instr_LEA (CPU *cpu);
	void instr_NOT (CPU *cpu);
	void instr_RTI (CPU *cpu);
	void instr_ST  (CPU *cpu);
	void instr_STI (CPU *cpu);
	void instr_STR (CPU *cpu);
	void instr_TRAP(CPU *cpu);
	int  readchar();             // TRAP reads a character for GETC, IN
	// *** STUB *** You may want other functions; depends on how you organize your code

// -------------------- MAIN PROGRAM --------------------
//
// The main program creates and initializes a CPU, loads a program,
// and executes it step by step (until it halts or the user quits).
// The CPU is dumped before and after executing the program
//
int main() {
	// Create and initialize the  CPU, read initial memory, dump CPU

	CPU cpu_struct, *cpu;
	cpu = &cpu_struct;
	init_CPU(cpu);
	populateMemory(CPU *cpu);
	while(cpu->running==1)
    {
        fetch_instr(CPU* cpu);
        instruction_cycle(CPU* cpu);

        //do checks to disable running maybe
        inc_instr(CPU* cpu);
    }


	/* *** STUB *** */
}

// -------------------- CPU ROUTINES --------------------
//
//

// init_CPU(cpu) simulates power-on initialization: Program counter
// and instruction register are set to 0, as are memory and registers;
// condition code = Z, and the CPU will run the instruction cycle.
//
void init_CPU(CPU *cpu) {
	cpu->pgm_counter = 0;
	cpu->instr_reg = 0;
	// *** STUB *** cpu->condition_code = ???;   // Z condition code on power-up
	cpu->running = 1;                // instruction cycle is running
    int regNbr;
	for (regNbr = 0; regNbr < NBR_REGS; regNbr++)
    {
		cpu->reg[regNbr] = 0;
        regNbr++;
	}

	// Implementation note: You can't use a while loop
	// that checks for addr >= MEMLEN because addr is an
	// unsigned short and hence always <= MEMLEN.
	// (I.e., if addr = MEMLEN, then addr++ sets addr to 0.)
	//
	// In any case, we want the last word of memory to contain a HALT.
	//
	Address addr = 0;
	while (addr < MEMLEN-1)
    {
		cpu->memory[addr++] = 0;
	}
	// *** STUB ***
}

// -------------------- INSTRUCTION ROUTINES --------------------

// Fetch instruction:
//   Copy instruction pointed to by program counter to instruction register
//   Increment program counter (wraparound addresses).
//
void fetch_instr(CPU *cpu)
{
	cpu->instr_reg = cpu->memory[cpu->pgm_counter];
	//removed incrementation to inc_instr		// unsigned overflow will wraparound to 0
}


void inc_instr(CPU* cpu)
{
    cpu->pgm_counter = cpu->pgm_counter + 1;
}

// Execute an instruction cycle
//   Fetch an instruction
//   Decode the instruction opcode
//   Execute the instruction
//
void instruction_cycle(CPU *cpu)
{
	// Get current instruction to execute and its location,
	// echo them out.
	//
	Address old_pc = cpu->pgm_counter;

	printf("x%04hX: x%04hX  ", old_pc, cpu->instr_reg);

	// Execute instruction; set cpu running = 0 if CPU execution
	// is to stop (because of TRAP HALT or an internal error).
	//

	unsigned short int opcode = getInt(0,3, CPU *cpu);
	//case removed, as case is 1600% slower than if else
	if(opcode == 0)//0000
        instr_BR  (CPU *cpu);
	else if(opcode == 1)//0001
        instr_ADD (CPU *cpu);
    else if(opcode = 2)//0010
        instr_LD  (CPU *cpu);
    else if(opcode = 3)//0011
        instr_ST  (CPU *cpu);
    else if(opcode = 4)//0100
        instr_JSR (CPU *cpu);
    else if(opcode = 5)//0101
        instr_AND (CPU *cpu);
    else if(opcode = 6)//0110
        instr_LDR (CPU *cpu);
    else if(opcode = 7)//0111
        instr_STR (CPU *cpu);
    else if(opcode = 8)//1000
        instr_RTI (CPU *cpu);
    else if(opcode = 9)//1001
        instr_NOT (CPU *cpu);
    else if(opcode = 10)//1010
        instr_LDI (CPU *cpu);
    else if(opcode = 11)//1011
        instr_STI (CPU *cpu);
    else if(opcode = 12)//1100
        instr_JMP (CPU *cpu);
    else if(opcode = 13)//1101
        instr_err (CPU *cpu);
    else if(opcode = 14)//1110
        instr_LEA (CPU *cpu);
    else if(opcode = 15)//1111
        instr_TRAP(CPU *cpu);
    else
    {
        printf("Bad opcode: %d; quitting\n FUCK OFF\n", op(cpu->instr_reg));
		cpu->running = 0;
    }








}

// Execute branch instruction: Bitwise AND instruction's mask and
// cpu's condition code, branch iff result is nonzero.
//
// Echo kind of branch, current cc, whether or not the branch happened,
// and if so, the target of the branch.
//
void instr_BR(CPU *cpu) {
	printf("BR instruction; *** STUB ***\n");
}

/* *** STUB *** */


// Read and return a character from standard input.  User must
// enter return after the char.  Just pressing return causes '\n'
// to be returned.  Any extra characters after the first are ignored.
//
int read_char()
{
	char buffer[3] = "";
	fgets(buffer, sizeof buffer, stdin);
	return buffer[0];
}

void instr_ADD(CPU *cpu)
{
    Word dr = getreg(CPU *cpu, 4);
    Word sr = getreg(CPU *cpu, 7);
    if(getbit(cpu, 10)==1)
    {
        int c = getInt(11, 15, cpu);
        dr=sr+c;
    }
    else
    {
        Word sr2 = getreg(cpu, 13);
        dr = sr+sr2;
    }
}
void populateMemory(CPU *cpu)
{
    FILE *fp;
    printf("Please enter the name of the hex file with the .hex: ");
	char[] fname = scanf(fname);
	fp = fopen(fname, "r");
	int numcommands = fread(cpu->memory, 2, 65565, fp);
	fclose(fp);
}
//returns an unsigned short int that represents the bits in the current instruction_reg's word that we requested




/// HELPER FUNCTIONS
unsigned short int getInt(int left, int right, CPU *cpu)
{
    if (left >= 0 && (right <= 15 && right > 0))
    {
        unsigned short int returnage=0;
        unsigned short int mask = 0xffff;
        mask = mask >> (left);
        returnage = cpu->instr_reg & mask;
        returnage = cpu->instr_reg >> (15 - right);
        return returnage;
    }
}


//returns a pointer to the register based on its starting location in instr_reg
Word* getreg(CPU *cpu, int i)
{
    reg = getInt(i,i+2,cpu->instr_reg);
    if(reg=0)
        return *(cpu->reg[0]);
    else if(reg=1)
        return *(cpu->reg[1]);
    else if(reg=2)
        return *(cpu->reg[2]);
    else if(reg=3)
        return *(cpu->reg[3]);
    else if(reg=4)
        return *(cpu->reg[4]);
    else if(reg=5)
        return *(cpu->reg[5]);
    else if(reg=6)
        return *(cpu->reg[6]);
    else if(reg=7)
        return *(cpu->reg[7]);
    else
        return null;
}
int getbit(CPU *cpu, int i)
{
    return = getInt(i,i,cpu->instr_reg);
}
/* *** STUB *** */
