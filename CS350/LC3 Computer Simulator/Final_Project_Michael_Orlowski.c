/// CS 350, Final Project, Fall 2013
/// LC3 simulator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// Basic Declarations

	typedef short int Word;
	typedef unsigned short int Address;

	#define MEMLEN 65536
	#define NBR_REGS 8

    typedef struct
	{
		Word memory[MEMLEN];
		Word reg[NBR_REGS];  /// Note: "register" is a reserved word
		int condition_code;  /// tells me whether the last register I manipulated has negative, psoitive, or zero value
		Word instr_reg;
		Address pgm_counter;
		int running;         /// running = 1 iff CPU is executing instructions
	}   CPU;


    /// Prototypes

	/// CPU manipulation and status dump functions and COMMAND LOOP functions

	void init_CPU(CPU *cpu);
	//void dump_CPU(CPU *cpu); /// isn't this the same as dump_control_unit?
	void dump_control_unit(CPU *cpu);
	void dump_memory(CPU *cpu, Address from, Address to);
	//char get_condition_code(CPU *cpu);
	//void set_condition_code(CPU *cpu, Word value); ///I'm not using this, I'm doing it manually with if statements.
	void helpMsg();
	void populateMemory(CPU *cpu); ///reads in from file and sets memory addresses accordingly

	/// Instruction cycle functions

	void instruction_cycle(CPU *cpu);      /// Simulate entire instruction cycle
	void fetch_instr(CPU *cpu);            /// Simulate fetch instruction part of cycle, called in above function
	void inc_instr(CPU *cpu);


	/// Functions for accessing parts of an instruction

	//int op(Word w);        getInt CAN HANDLE THIS               /// Opcode of an instruction

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
	void instr_RET (CPU *cpu);
	void instr_RTI (CPU *cpu);
	void instr_ST  (CPU *cpu);
	void instr_STI (CPU *cpu);
	void instr_STR (CPU *cpu);
	void instr_TRAP(CPU *cpu);
	int  read_char(); /// TRAP reads a character for GETC, IN

	///HELPER FUNCTIONS

    Address getInt(int left, int right, CPU *cpu);
    Address getReg0(int left, int right, CPU *cpu);
	Address getreg(CPU *cpu, int i);


/// -------------------- MAIN PROGRAM --------------------
/// The main program creates and initializes a CPU, loads a program,
/// and executes it step by step (until it halts or the user quits).
/// The CPU is dumped before and after executing the program

int main()
{
	/// Create and initialize the  CPU, read initial memory, dump CPU
    CPU cpu_struct, *cpu;
	cpu = &cpu_struct;
	init_CPU(cpu);
	populateMemory(cpu); ///reads in the file with hex commands

	char dummy[5];
	fgets(dummy, sizeof dummy, stdin);///dummy read-in, otherwise the carriage return hit after the file name would go promptly into an instruction-cycle

	char prompt[] = "> ";

    /// PROMPTING THE USER FOR COMMANDS

    printf("\nBeginning execution:\n");
	printf("At the %sprompt, press return to execute the next instruction\n", prompt);
	printf("or d to dump the control unit\n");
    printf("or q to quit\n");
    printf("or m n1 n2 (in hex!) to print out registers n1, n1+1,..., n2\n");
    printf("or h or ? for help\n");
    printf("or an integer to execute the instruction cycle the amount of times you want.\n");

    char command[80]; ///user's command
	int commands_done = 0; ///boolean for command loop

    while (commands_done == 0) /// ENTERING COMMAND LOOP
    {
        printf("\n");
        printf("%s", prompt);

        fgets(command, sizeof command, stdin);   /// GET USER'S COMMAND

        int instr_Cycle_Num;
        sscanf(command, "%d", &instr_Cycle_Num); /// typecasts the integer char into an int to execute the instr cycle

        if (strcmp(command, "d\n") == 0)
            dump_control_unit(cpu);

        else if (command[0] == 'm')
        {
            char trash;
            Address from, to;
            sscanf(command, "%c %hx %hx", &trash, &from, &to);
            dump_memory(cpu, from, to);
        }
        else if ((strcmp(command, "h\n") == 0) || (strcmp(command, "?\n") == 0))
            helpMsg();

        else if ((strcmp(command, "\n") == 0))
        {
                if (cpu->running == 1)
            {

                instruction_cycle(cpu);
            }
            else
                printf("NOT RUNNING\n");
        }
        else if (instr_Cycle_Num != 0)
        {
            printf("Executing multiple times\n");

            while (instr_Cycle_Num > 0)
            {
                instruction_cycle(cpu);
                instr_Cycle_Num--;
            }

        }

        else if(strcmp(command, "q\n") == 0)
        {
            commands_done = 1;
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
	cpu->instr_reg = 0;
	cpu->running = 1;    /// instruction cycle is running
	cpu->condition_code = 2;

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
	 cpu->memory[MEMLEN-1] = 0xf025;  ///HALT
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

void instruction_cycle(CPU *cpu)
{
	/// Get current instruction to execute and its location, echo them out.
    fetch_instr(cpu);

	Address old_pc = cpu->pgm_counter;

	printf("x%04hx: x%04hx  \n", old_pc, cpu->instr_reg);

	/// Execute instruction; set cpu running = 0 if CPU execution
	/// is to stop (because of TRAP HALT or an internal error).

    ///   Decode the instruction opcode
	Address opcode = getInt(0,3,cpu);

	printf("OPCODE: %d\n", opcode);

    ///   Execute the instruction
	if(opcode == 0)        //0000
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
        printf("Incorrect opcode: %d; quitting", opcode); ///was op(cpu->instr_reg)
		cpu->running = 0;
    }

    inc_instr(cpu); /// inc the program countercp
}


/// INSTRUCTION FUNCTIONS

void instr_ADD(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address destinationAddress = getreg(cpu, 4);
    Address sourceAddress = getreg(cpu, 7);

    if(( ((cpu->instr_reg)>>5) & 0x0001) == 1)  ///checks if bit at position 10 is 1, or 0
    {

        Address immediate = getInt(11, 15, cpu);
        cpu->reg[destinationAddress] = cpu->reg[sourceAddress] + (Word) immediate;
        printf("ADD: R%d <- R%d + immediate (%d)\n", destinationAddress, sourceAddress, immediate);
        if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
        if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
        if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
    }
    else
    {
        Address sourceAddress2 = getreg(cpu, 13);
        cpu->reg[destinationAddress] = cpu->reg[sourceAddress] + cpu->reg[sourceAddress2];
        printf("ADD: R%d <- R%d + R%d = %d\n", destinationAddress, sourceAddress, sourceAddress2, cpu->reg[sourceAddress] + cpu->reg[sourceAddress2]);
        if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
        if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
        if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
    }
}

void instr_AND(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address destinationAddress = getreg(cpu, 4);
    Address source1Address = getreg(cpu, 7);

    if ((((cpu->instr_reg)>>5)&0x0001) == 0)
    {
        Address source2Address = getreg(cpu, 13);
        cpu->reg[destinationAddress] = cpu->reg[source1Address] & cpu->reg[source2Address];
        printf("AND: R%d <- R%d & R%d = %d \n", destinationAddress, source1Address, source2Address, cpu->reg[source1Address] & cpu->reg[source2Address]);
        if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
        if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
        if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
    }
    else if ((((cpu->instr_reg)>>5)&0x0001) == 1)
    {
        Word value = cpu->reg[source1Address] & (Word) getInt(11, 15, cpu);
        cpu->reg[destinationAddress] = value;
        printf("AND: R%d <- R%d & immediate (%d) = %d", destinationAddress, source1Address, value , cpu->reg[source1Address] & (Word) getInt(11, 15, cpu));
        if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
        if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
        if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
    }

}

void instr_JMP(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address base = getInt(7, 9, cpu);
    cpu->pgm_counter = cpu->reg[base];
    printf("JMP: PC <- R%d = %d\n", base, cpu->reg[base]);
}

void instr_JSR(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Word offset = (Word) getInt(5, 15, cpu);
    if ( ((offset >> 11) & 0x0001) == 1)
    {
        Address base = getInt(7, 9, cpu);
        cpu->reg[7] = cpu->pgm_counter;
        cpu->pgm_counter = cpu->reg[base];
        printf("JSRR: R7 <- PC = %d ; ", cpu->pgm_counter);
        printf("PC <- R%d\n", base);
    }
    else
    {
        cpu->reg[7] = cpu->pgm_counter;
        if ( ((offset >> 10) & 0x0001) == 1)
        {
            offset = -(~(offset) + 1);
        }
        cpu->pgm_counter = cpu->pgm_counter + offset;
        printf("JSR: R7 <- PC = %d  ;", cpu->pgm_counter);
        printf(" PC <- PC + offset = %d\n", cpu->pgm_counter + offset );

    }
}

void instr_LD(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Word destinationAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->reg[destinationAddress] = cpu->memory[offset + cpu->pgm_counter];
    printf("LD: R%d <- Memory[(%d) offset + (%d) PC] = %d\n", destinationAddress, offset,  cpu->pgm_counter, cpu->memory[offset + cpu->pgm_counter]);
    if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
    if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
    if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
}

void instr_LDI(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address destinationAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->reg[destinationAddress] = cpu->memory[cpu->memory[cpu->pgm_counter + offset]];
    printf("LDI: R%d <- Memory[Memory[%d (offset) + %d (PC) ] (%d)] = %d\n", destinationAddress, offset, cpu->pgm_counter, cpu->memory[cpu->pgm_counter + offset], cpu->memory[cpu->memory[cpu->pgm_counter + offset]] );
     if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
    if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
    if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
}

void instr_LDR(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address base = getInt(7, 9, cpu);
    Address destinationAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(10, 15, cpu);
    if ( ((offset >> 4) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->reg[destinationAddress] = cpu->memory[base + offset];
    printf("LDR: R%d <- Memory[base (%d) + offset (%d)] = %d\n", destinationAddress, base, offset, cpu->memory[base + offset]);
     if (cpu->reg[destinationAddress] > 0);
            cpu->condition_code = 1;
    if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
    if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
}

void instr_LEA(CPU *cpu)
{
     printf("\n");
     printf(">> ");
     Address destinationAddress = getreg(cpu, 4);
     Word offset = (Word) getInt(7, 15, cpu);
     if ( ((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
     cpu->reg[destinationAddress] = cpu->pgm_counter + offset;
     printf("LEA: R%d <- PC (%d) + offset (%d) = %d\n", destinationAddress, cpu->pgm_counter, offset, cpu->pgm_counter + offset);
    if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
    if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
    if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;

}

void instr_NOT(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address sourceAddress = getreg(cpu, 7);
    Address destinationAddress = getreg(cpu, 4);
    Word notSource = ~cpu->reg[sourceAddress];
    notSource++; /// have to add 1 for  two's complement
    cpu->reg[destinationAddress] = notSource;
    printf("NOT: R%d <-  ~ R%d = %d\n", destinationAddress, sourceAddress, notSource);
    if (cpu->reg[destinationAddress] > 0)
            cpu->condition_code = 1;
    if (cpu->reg[destinationAddress] < 0)
            cpu->condition_code = 4;
    if (cpu->reg[destinationAddress] == 0)
            cpu->condition_code = 2;
}

void instr_RET(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    cpu->pgm_counter = cpu->reg[7];
    printf("RET: PC <- R7 = %d\n",  cpu->reg[7]);
}

void instr_RTI(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    printf("RTI: Goodbye CS350.\n");
}

void instr_ST(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address sourceAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if (((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[(cpu->pgm_counter) + offset] = cpu->reg[sourceAddress];
    printf("ST: Memory[PC + offset]%d <- R%d = %d", cpu->pgm_counter+offset, sourceAddress, cpu->reg[sourceAddress]);
}

void instr_STI(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address sourceAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(7, 15, cpu);
    if (((offset >> 8) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[cpu->memory[cpu->pgm_counter + offset]] = cpu->reg[sourceAddress];

    printf("STI: Memory[Memory[PC + offset] (%d)] <- R%d = %d\n", cpu->memory[cpu->pgm_counter + offset], sourceAddress, cpu->reg[sourceAddress]);
}

void instr_STR(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    Address base = getInt(7, 9, cpu);
    Address sourceAddress = getreg(cpu, 4);
    Word offset = (Word) getInt(10, 15, cpu);
    if (((offset >> 5) & 0x0001) == 1)
    {
        offset = -(~(offset) + 1);
    }
    cpu->memory[base + offset] = cpu->reg[sourceAddress];
    printf("STR: Memory[Memory[PC + offset] (%d)] <- R%d = %d", cpu->memory[cpu->pgm_counter + offset], sourceAddress, cpu->reg[sourceAddress]);
}

void instr_TRAP(CPU *cpu)
{
    printf("\n");
    printf(">> ");
    cpu->reg[7] = cpu->pgm_counter;
    Address vectr = getInt(8, 15, cpu); ///gets vector
    cpu->pgm_counter = vectr;

    if (vectr == 32) /// x20 = 0010 0000
    {
        printf("GETC: \n");
        int input = read_char();
        Word newWordReg0 = (Word) input;
        cpu->reg[0] = newWordReg0;
    }
    else if (vectr == 33) /// x21 = 0010 0001
    {
        printf("OUT: \n");
        Address rightByte = getReg0(8, 15, cpu);
        printf("Right byte: %d, Whole number: %d\n", rightByte, cpu->reg[0]);
        printf("%c\n", rightByte);
    }
    else if (vectr == 34) /// x22 = 0010 0010
    {
        printf("PUTS: \n");
        Word read;
        int offset = 1;
        read = cpu->memory[cpu->reg[0]+offset];

        while(read!=0)
        {
            printf("%c", read);
            offset++;
            read = cpu->memory[cpu->reg[0]+offset];
        }
        printf("\n");

    }
    else if (vectr == 35) /// x23 = 0010 0011
    {
        printf("IN: \n");
        printf("Enter a character: \n");
        int input = read_char();
        Word newWordReg0 = (Word) input;
        cpu->reg[0] = newWordReg0;

    }
    else if (vectr == 36) /// x25 = 0010 0101
    {
        printf("HALT: \n");
        cpu->running = 0; ///flag now false
    }

}

void instr_err (CPU *cpu)
{
    printf("\n");
    printf(">> ");
    printf("Instruction error, reserved oppcode processed: halting.\n");
    cpu->running = 0;
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
	if ((cpu->condition_code & nzpInstr) > 0) ///branches if negative, positive, and zero
    {
        Word offset = (Word) getInt(7, 15, cpu);
        if (((offset >> 8) & 0x0001) == 1) ///determines whether the offset is negative
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

/// COMMAND LOOP INSTRUCTIONS

void helpMsg()
{
	 ///PRINTING THE COMMAND LOOP INSTRUCTIONS

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
    printf("Memory values: \n");
    for (from; from <= to; from++)
    {
        if (!(cpu->memory[from] == 0))
        {
            printf("Memory at location %d = %hx \n", from, cpu->memory[from]);
        }
    }
}


void dump_control_unit(CPU *cpu)
{
        int i = 0;
        int j = 0;
        int rCount=0;

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

      for (j = 0; j < 8; j++)
	{
	  printf("R%d: %04hx ", rCount, cpu->reg[rCount]);
	  rCount++;
	  if(cpu->reg[i+j+1]>=-999)
        printf(" ");
	}
      printf("\n");

}

/// HELPER FUNCTIONS

void populateMemory(CPU *cpu)
{
    FILE *fp;
    Address i = 0;
    char fname [16]; /// assuming we know the size of the name
    printf("Please enter the .hex file that you would like to read in (with the .hex!): ");
    scanf("%s", &fname);
    fp = fopen(fname, "r");
    char firstLine[6];
    if (fp != NULL)
    {
        fgets(firstLine, sizeof firstLine, fp); ///reads in first line from file
        sscanf(firstLine, "%hx", &i); ///sets memory address
        cpu->pgm_counter = i; ///sets program counter to that memory address

        char line [6];
        while (fgets(line, sizeof line, fp)!= NULL) /// reads rest of the lines
            {
                sscanf(line, "%hx", &cpu->memory[i]);
                i++;
            }
    }
    else
    {
        printf("There was an error opening the file. You might have typed the incorrect file name.\n");
    }

}

Address getInt(int left, int right, CPU *cpu)  ///This is the bitmasking code from lab5
{                                              ///This will allow me to get the opcode or any other desired bitstring of at most length 16 (0,...,15)
    Address usersHex = cpu->instr_reg;
    if (left >= 0 && (right <= 15 && right > 0))
        {
            unsigned int mask = 0xffff;
            mask = mask >> (left);
            usersHex = usersHex & mask;
            usersHex = usersHex >> (15 - right);
        }
        return usersHex;
}

Address getReg0(int left, int right, CPU *cpu) ///Used for PUTS, like getInt but for cpu->reg[0]
{
    Address usersHex = cpu->reg[0];
    if (left >= 0 && (right <= 15 && right > 0))
        {
            unsigned int mask = 0xffff;
            mask = mask >> (left);
            usersHex = usersHex & mask;
            usersHex = usersHex >> (15 - right);
        }
        return usersHex;
}

Address getreg(CPU *cpu, int i) /// Much like getInt, but just for getting 3 bit long integers
{
    int left=i, right = i+2;
    Address usersHex = cpu->instr_reg;
    if (left >= 0 && (right <= 15 && right > 0))
        {
            unsigned int mask = 0xffff;
            mask = mask >> (left);
            usersHex = usersHex & mask;
            usersHex = usersHex >> (15 - right);
        }
    Address reg =  usersHex;
    if(reg == 0)
        return 0;          ///returns addresses
    else if(reg == 1)
        return 1;
    else if(reg == 2)
        return 2;
    else if(reg == 3)
        return 3;
    else if(reg == 4 )
        return 4;
    else if (reg == 5)
        return 5;
    else if (reg == 6)
        return 6;
    else if (reg == 7)
        return 7;
    else
        return -1;
}
