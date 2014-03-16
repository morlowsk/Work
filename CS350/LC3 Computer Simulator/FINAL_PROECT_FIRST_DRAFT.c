/// CS 350, Final Project, Fall 2013
/// LC3 simulator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// Basic Declarations

	typedef short int Word;
	typedef unsigned short int Address;

	#define MEMLEN 65536
	#define NBR_REGS 7 //was 8

    typedef struct
	{
		Word memory[MEMLEN];
		Word reg[NBR_REGS];  /// Note: "register" is a reserved word
		int condition_code;  /// positive, zero, or negative  /// tells me whether the last register I manipulated has negative, psoitive, or zero value
		Word instr_reg;
		Address pgm_counter;
		int running;         /// running = 1 iff CPU is executing instructions
	}   CPU;


    /// Prototypes

	/// CPU manipulation and status dump functions and COMMAND LOOP functions

	void init_CPU(CPU *cpu);
	//void dump_CPU(CPU *cpu);
	void dump_control_unit(CPU *cpu);
	void dump_memory(CPU *cpu, Address from, Address to);
	//char get_condition_code(CPU *cpu);  /// typecast the condition code
	//void set_condition_code(CPU *cpu, Word value); /// same here ///
	void helpMsg();
	void populateMemory(CPU *cpu);

	/// Instruction cycle functions

	void instruction_cycle(CPU *cpu);      /// Simulate entire instruction cycle
	void fetch_instr(CPU *cpu);            /// Simulate fetch instruction part of cycle, called in above function
	void inc_instr(CPU *cpu);


	/// Functions for accessing parts of an instruction

	//int op(Word w);                       /// Opcode of an instruction

	/// Functions for executing each instruction

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
	void instr_RET (CPU *cpu); ///added this here
	void instr_RTI (CPU *cpu);
	void instr_ST  (CPU *cpu);
	void instr_STI (CPU *cpu);
	void instr_STR (CPU *cpu);
	void instr_TRAP(CPU *cpu);
	int  readchar(); /// TRAP reads a character for GETC, IN

	///HELPER FUNCTIONS

    Address getInt(int left, int right, CPU *cpu);
	Word getreg(CPU *cpu, int i);


/// -------------------- MAIN PROGRAM --------------------
/// The main program creates and initializes a CPU, loads a program,
/// and executes it step by step (until it halts or the user quits).
/// The CPU is dumped before and after executing the program

int cycleFlag;

int main()
{
	/// Create and initialize the  CPU, read initial memory, dump CPU
    CPU cpu_struct, *cpu;
	cpu = &cpu_struct;
	init_CPU(cpu);
	populateMemory(cpu); ///reads in the file with hex commands
	char prompt[] = "> ";
    printf("\nBeginning execution:\n");
	printf("At the %sprompt, press return to execute the next instruction\n", prompt);
	printf("or d to dump the control unit\n");
    printf("or q to quit\n");
    printf("or m n1 n2 (in hex!) to print out registers n1, n1+1,..., n2\n");
    printf("or h or ? for help, or an integer to execute the instruction cycle the amount of times you want.\n");
    char command[80];          /// fgets(command, sizeof command, stdin);
	int commands_done = 0;                                               ///int instr_Cycle_Num  = atoi(command);
    while (commands_done==0) /// ENTERING COMMAND LOOP
    {
        printf("\n");
        printf("%s", prompt);
        fgets(command, sizeof command, stdin);   /// Read through end of current line.
        int instr_Cycle_Num;
        sscanf(command, "%d", &instr_Cycle_Num);
        if(strcmp(command, "q\n") == 0)
        {
            commands_done = 1;
        }

        else if (strcmp(command, "d\n") == 0)
            dump_control_unit(cpu);

        else if (command[0] == 'm')
        {
            char garbage;
            Address from, to;
            sscanf(command, "%c %hx %hx", &garbage, &from, &to);
            dump_memory(cpu, from, to);
        }
        else if ((strcmp(command, "h\n") == 0) || (strcmp(command, "?\n") == 0))
            helpMsg();
        else if ((strcmp(command, "\n") == 0))
        {
                while(cpu->running == 1)
            {
               // printf("I'M A HUGE FAGGOT!");
                cycleFlag = 1;
                instruction_cycle(cpu);
            }
        }
        else if (instr_Cycle_Num != 0)
        {
            while (instr_Cycle_Num > 0 )
            {
                printf("I'M A HUGE FAGGOT!");
                instruction_cycle(cpu);
                instr_Cycle_Num--;
            }

        }
        else
            printf("Wrong command. Try again.");

    }

return 0;

}

/// -------------------- CPU ROUTINES --------------------
/// init_CPU(cpu) simulates power-on initialization: Program counter
/// and instruction register are set to 0, as are memory and registers;
/// condition code = Z, and the CPU will run the instruction cycle.

void init_CPU(CPU *cpu)
 {
	cpu->pgm_counter = 0;
	cpu->instr_reg = 0;   /// *** STUB *** cpu->condition_code = ???;   // Z condition code on power-up
	cpu->running = 1;    /// instruction cycle is running
	cpu->condition_code = 2;
    int regNbr;
	for (regNbr = 0; regNbr < NBR_REGS; regNbr++)
    {
		cpu->reg[regNbr] = 0;
        regNbr++;
	}

    /// WHAT?
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
	/// cpu->memory[MEMLEN-1] = ;  ///HALT
}

///   -------------------- INSTRUCTION ROUTINES --------------------
///   Fetch instruction:
///   Copy instruction pointed to by program counter to instruction register
///   Increment program counter (wraparound addresses).

void fetch_instr(CPU *cpu) ///kind of useless function
{
	cpu->instr_reg = cpu->memory[cpu->pgm_counter]; /// unsigned overflow will wraparound to 0
}

void inc_instr(CPU* cpu)
{
    cpu->pgm_counter = cpu->pgm_counter + 1;
}

///   Execute an instruction cycle
///   Fetch an instruction
///   Decode the instruction opcode
///   Execute the instruction

void instruction_cycle(CPU *cpu)
{
	/// Get current instruction to execute and its location, echo them out.
    fetch_instr(cpu);

	Address old_pc = cpu->pgm_counter;

	printf("x%04hX: x%04hX  ", old_pc, cpu->instr_reg);

	/// Execute instruction; set cpu running = 0 if CPU execution
	/// is to stop (because of TRAP HALT or an internal error).

	Address opcode = getInt(0,3,cpu);     /// switch case removed, as case is 1600% slower than if else

	if(opcode == 0)       //0000
        instr_BR(cpu);
	else if(opcode == 1)   //0001
        instr_ADD(cpu);
    else if(opcode == 2)   //0010
        instr_LD(cpu);
    else if(opcode == 3)   //0011
        instr_ST(cpu);
    else if(opcode == 4)   //0100
        instr_JSR(cpu);
    else if(opcode == 5)   //0101
        instr_AND(cpu);
    else if(opcode == 6)   //0110
        instr_LDR(cpu);
    else if(opcode == 7)   //0111
        instr_STR(cpu);
    else if(opcode == 8)   //1000
        instr_RTI(cpu);
    else if(opcode == 9)   //1001
        instr_NOT(cpu);
    else if(opcode == 10)  //1010
        instr_LDI(cpu);
    else if(opcode == 11)  //1011
        instr_STI(cpu);
    else if(opcode == 12)  //1100
        instr_JMP(cpu);
    else if(opcode == 13)  //1101
        instr_err(cpu);
    else if(opcode == 14)  //1110
        instr_LEA(cpu);
    else if(opcode == 15)  //1111
        instr_TRAP(cpu);
    else
    {
       /// printf("Bad opcode: %d; quitting\n FUCK OFF\n", op(cpu->instr_reg));
		cpu->running = 0;
    }

    inc_instr(cpu); /// inc the program counter
}


/// INSTRUCTION FUNCTIONS

void instr_ADD(CPU *cpu)
{
    printf("\n");
    printf(">");
    Word destination = getreg(cpu, 4);
    Word source = getreg(cpu, 7);

    if((((cpu->instr_reg)>>5)&0x0001) == 1)
    {

        Address immediate = getInt(11, 15, cpu);
        destination = source + (Word) immediate;
        printf("ADD R%d <- R%d + immediate (%d)\n", destination, source, immediate);
        if (destination > 0);
            cpu->condition_code = 1;
        if (destination < 0)
            cpu->condition_code = 4;
        if (destination == 0)
            cpu->condition_code = 2;
    }
    else
    {
        Word source2 = getreg(cpu, 13);
        destination = source + source2;
        printf("ADD R%d <- R%d + R%d \n", destination, source, source2);
        if (destination > 0)
            cpu->condition_code = 1;
        if (destination < 0)
            cpu->condition_code = 4;
        if (destination == 0)
            cpu->condition_code = 2;
    }
}

void instr_AND(CPU *cpu)
{
    printf("\n");
    printf(">");
    Word destination = getreg(cpu, 4);
    Word source1 = getreg(cpu, 7);

    if ((((cpu->instr_reg)>>5)&0x0001) == 0)
    {
        Word source2 = getreg(cpu, 13);
        destination = source1 & source2;
        printf("AND: R%d <- R%d & R%d) \n", destination, source1, source2);
        if (destination > 0)
            cpu->condition_code = 1;
        if (destination < 0)
            cpu->condition_code = 4;
        if (destination == 0)
            cpu->condition_code = 2;
    }
    else if ((((cpu->instr_reg)>>5)&0x0001) == 1)
    {
        Word value = source1 & getInt(11, 15, cpu);
        destination = value;
        printf("AND: R%d <- R%d & immediate (%d)", destination, source1, value);
        if (destination > 0)
            cpu->condition_code = 1;
        if (destination < 0)
            cpu->condition_code = 4;
        if (destination == 0)
            cpu->condition_code = 2;
    }

}

void instr_JMP(CPU *cpu)
{
    printf("\n");
    printf(">");
    Address base = getInt(7, 9, cpu);
    cpu->pgm_counter = cpu->reg[base];
    printf("JMP PC <- R%d \n", cpu->pgm_counter, base);
}

void instr_JSR(CPU *cpu)
{
    printf("\n");
    printf(">");
    cpu->reg[7] = cpu->pgm_counter;
    Word offset = (Word) getInt(5, 15, cpu);
    if ( ((offset >> 10) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->pgm_counter = cpu->pgm_counter + offset;
    printf("JSR PC <- PC + offset (%d)\n", offset);
    printf("JSR R7 <- PC\n");
}

void instr_JSRR(CPU *cpu)
{
    printf("\n");
    printf(">");
    Address base = getInt(7, 9, cpu);
    cpu->reg[7] = cpu->pgm_counter;
    cpu->pgm_counter = cpu->reg[base];
    printf("JSRR R7 <- PC\n");
    printf("JSRR PC <- R%d\n", base);
}

void instr_LD(CPU *cpu)
{
    printf("\n");
    printf(">");
    Word destination = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu); ///do getInt with bigger mask
    if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    destination = cpu->memory[offset + cpu->pgm_counter];
    printf("LD R%d <- Memory[%d (offset) + PC]\n", getInt(4,6, cpu), offset);
    if (destination > 0)
            cpu->condition_code = 1;
    if (destination < 0)
            cpu->condition_code = 4;
    if (destination == 0)
            cpu->condition_code = 2;
}

void instr_LDI(CPU *cpu)
{
    printf("\n");
    printf(">");
    Word destination = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    destination = cpu->memory[cpu->memory[cpu->pgm_counter + offset]];
    printf("LDI R%d <- Memory[Memory[%d (offset) + %d (PC) ]]\n", getInt(4,6, cpu), offset, cpu->pgm_counter);
    if (destination > 0)
            cpu->condition_code = 1;
    if (destination < 0)
            cpu->condition_code = 4;
    if (destination == 0)
            cpu->condition_code = 2;
}

void instr_LDR(CPU *cpu)
{
    printf("\n");
    printf(">");
    Address base = getInt(7, 9, cpu);
    Word destination = getreg(cpu, 4);
    Address offset = (Word) getInt(10, 15, cpu);
    if ( ((offset >> 4) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    destination = cpu->memory[base + offset];
    printf("LDR R%d <- Memory[base (%d) + offset (%d)] = %d\n", getInt(4,6, cpu), base, offset, cpu->memory[base + offset]);
    if (destination > 0)
            cpu->condition_code = 1;
    if (destination < 0)
            cpu->condition_code = 4;
    if (destination == 0)
            cpu->condition_code = 2;
}

void instr_LEA(CPU *cpu)
{
     printf("\n");
     printf(">");
     Word destination = getreg(cpu, 4);
     Word offset = (Word) getInt(7, 15, cpu);
     if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
     destination = cpu->pgm_counter + offset;
     printf("LEA R%d <- PC (%d) + offset (%d) = %d\n", getInt(4,6, cpu), cpu->pgm_counter, offset, cpu->pgm_counter + offset);
     if (destination > 0)
            cpu->condition_code = 1;
     if (destination < 0)
            cpu->condition_code = 4;
     if (destination == 0)
            cpu->condition_code = 2;

}

void instr_NOT(CPU *cpu)
{
    printf("\n");
    printf(">");
    Word source = getreg(cpu, 7);
    Word destination = getreg(cpu, 4);
    Word notSource = ~source;
    notSource++; /// have to add 1
    destination = notSource;
    printf("NOT R%d <-  - R%d = %d\n", getInt(4,6, cpu), getInt(7,9, cpu), notSource);
    if (destination > 0)
            cpu->condition_code = 1;
    if (destination < 0)
            cpu->condition_code = 4;
    if (destination == 0)
            cpu->condition_code = 2;

}

void instr_RET(CPU *cpu)
{
    printf("\n");
    printf("> ");
    cpu->pgm_counter = cpu->reg[7];
    printf("PC <- R7 = %d\n",  cpu->reg[7]);
}

void instr_RTI(CPU *cpu)
{
    printf("\n");
    printf("> ");
    printf("RTI Goodbye CS350.\n");
}

void instr_ST(CPU *cpu)
{
    printf("\n");
    printf("> ");
    Word source = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if (((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[cpu->pgm_counter + offset] = source;
    printf("ST Memory[PC + offset] <- R%d = %d", getInt(4,6,cpu), source);
}

void instr_STI(CPU *cpu)
{
    printf("\n");
    printf("> ");
    Word source = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if (((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[cpu->memory[cpu->pgm_counter + offset]] = source;

    printf("STI Memory[Memory[PC + offset] (%d)] <- R%d = %d\n", cpu->memory[cpu->pgm_counter + offset] ,getInt(4,6,cpu), source);
}

void instr_STR(CPU *cpu)
{
    printf("\n");
    printf("> ");
    Address base = getInt(7, 9, cpu);
    Word source = getreg(cpu, 4);
    Word offset = (Word) getInt(10, 15, cpu);
    if (((offset >> 5) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[base + offset] = source;
    printf("STR Memory[Memory[PC + offset] (%d)] <- R%d = %d", cpu->memory[cpu->pgm_counter + offset], getInt(4,6,cpu), source);
}

void instr_TRAP(CPU *cpu)
{
    printf("\n");
    printf("> ");
    cpu->reg[7] = cpu->pgm_counter;
    Address vectr = getInt(8, 15, cpu);
    cpu->pgm_counter = vectr;

    if (vectr == 32) /// x20 = 0010 0000
    {
        printf("GETC\n\n");
        char input;
        scanf("%c", input);

        Word newWordReg0 = (Word) input;
        cpu->reg[0] = newWordReg0;

    }
    else if (vectr == 33) /// x21 = 0010 0001
    {
        printf("OUT\n");
        printf("%d", cpu->reg[0]);
    }
    else if (vectr == 34) /// x22 = 0010 0010
    {
        /// WORK ON
        printf("PUTS\n"); ///ASK
    }
    else if (vectr == 35) /// x23 = 0010 0011
    {
        printf("IN\n");
        char input;
        printf("Enter a character: \n");
        scanf("%c", input);
        char newReg0[16] = {0, 0, 0, 0, 0, 0, 0, 0, input};
        int newIntReg0;
        sscanf(newReg0, "%d", &newIntReg0);
        Word newWordReg0 = (Word) newIntReg0;
        cpu->reg[0] = newWordReg0;

    }
    else if (vectr == 36) /// x25 = 0010 0101
    {
        printf("HALT\n");
        cycleFlag = 0; ///flag is set to false
    }

}

void instr_err (CPU *cpu)
{
    ///NOT DONE!
    printf("\n");
    printf("> ");
    printf("do some shit.\n");
}

/// Execute branch instruction: Bitwise AND instruction's mask and
/// cpu's condition code, branch iff result is nonzero.
///
/// Echo kind of branch, current cc, whether or not the branch happened,
/// and if so, the target of the branch.

void instr_BR(CPU *cpu)
{
	printf("\n");
    printf("> ");
	int nzpInstr = (int) getInt(4, 6, cpu);
	if ((cpu->condition_code & nzpInstr) > 0)
    {
        Word offset = (Word) getInt(7, 15, cpu);
        if (((offset >> 8) & 0x0001) == 1)
        {
            offset = -(~(offset) + 1);
        }
        cpu->pgm_counter = cpu->pgm_counter + offset;
        printf("BR: PC <- PC (%d) + offset (%d) = %d", cpu->pgm_counter , offset, cpu->pgm_counter + offset);
	}
}

/// Read and return a character from standard input.  User must
/// enter return after the char.  Just pressing return causes '\n'
/// to be returned.  Any extra characters after the first are ignored.

int read_char()
{
	char buffer[3] = "";
	fgets(buffer, sizeof buffer, stdin);
	return buffer[0];
}


/// returns an unsigned short int that represents the bits in the current instruction_reg's word that we requested

/// COMMAND LOOP INSTRUCTIONS

void helpMsg()
{
	 ///print the instruction set

     printf("Commands:\n\n");
     printf("Enter 'q' to quit. \n");
     printf("Enter an integer to execute the number of instruction cycles you would like.\n");
     printf("Enter 'm' followed by two hexadecimal numbers to dump memory from those first memory location you entered to the second.\n");
     printf("Enter 'h' or '?' for simulator commands.\n");
     printf("Press 'enter'  to execute an instruction cycle.\n");


}

void dump_memory(CPU *cpu, Address from, Address to)
{
    ///JUST PRINT OUT MEMORY VALUES LINE BY LINE
     printf("Memory values: \n"); /// this is giving me a bug, why?
    for (from; from <= to; from++)
    {
        if (!(cpu->memory[from] == 0))
        {
            printf("Memory at location %d = %x \n", from, cpu->memory[from]);
        }
    }
}


void dump_control_unit(CPU *cpu)
{
    printf("Dump Control Unit\n\n");
	printf("PC: ");
	if(cpu->pgm_counter >= -999)
        printf(" ");
    printf("%d  IR: ", cpu->pgm_counter);
    if(cpu->instr_reg >= -999)
        printf(" ");
    printf("%d\n\n", cpu->instr_reg);
    printf("Condition Code: ");
    if(cpu->condition_code >= -999)
        printf(" ");
    printf("%d \n\n", cpu->condition_code);
///PRINTING REGISTERS
    int i = 0;
    int j = 0;
    int rCount=0;

      for (j = 0; j < 9; j++)
	{
	  printf("R%d: %04d ", rCount, cpu->reg[rCount]);
	  rCount++;
	  if(cpu->reg[i+j+1]>=-999)
        printf(" ");
	}
      printf("\n");

}


/// HELPER FUNCTIONS

void populateMemory(CPU *cpu)
{
    int j = 0;
    FILE *fp;

    int i = 0;
    char fname [16]; /// assuming we know the size of the name
    printf("Please enter the name of the hex file with the .hex: ");
    scanf("%s", fname);
	fp = fopen(fname, "r"); /// do a loop
	if (fp != NULL)
    {
        char line [16];
        while (fgets(line, sizeof line, fp)!= NULL) /// read a line
            {
                sscanf(line, "%hx", &cpu->memory[i]); ///was fscanf(fp, "%x", cpu->memory[i]);
                i++;
            }
        j = 1;
    }
    else
    {
        /// perror(fp);
        printf("You might have typed the incorrect file name.\n");
        printf("Try again: ");
        scanf("%s", fp);
    }


	fclose(fp);
}

Address getInt(int left, int right, CPU *cpu)
{

    if (left >= 0 && (right <= 15 && right > 0))
    {
        Address returnage = 0;
        Address mask = 0xffff;
        mask = mask >> (left);
        returnage = cpu->instr_reg & mask;
        returnage = cpu->instr_reg >> (15 - right);
        return returnage;
    }
}


/// returns a pointer to the register based on its starting location in instr_reg

Word getreg(CPU *cpu, int i)
{
    Address reg =  getInt(i,i+2,cpu);
    if(reg == 0)
        return (cpu->reg[0]);
    else if(reg == 1)
        return (cpu->reg[1]);
    else if(reg == 2)
        return (cpu->reg[2]);
    else if(reg == 3)
        return (cpu->reg[3]);
    else if(reg == 4 )
        return (cpu->reg[4]);
    else if (reg == 5)
        return (cpu->reg[5]);
    else if (reg == 6)
        return (cpu->reg[6]);
    else if(reg == 7)
        return (cpu->reg[7]);
    else
        return 0;
}


