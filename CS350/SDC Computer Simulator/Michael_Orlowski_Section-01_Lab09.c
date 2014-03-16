// Michael Orlowski - Section 01 CS 350

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Prototypes
    void helpMsg(void);
	void initCPU(void);
    void readMemory(void);
    void dumpControlUnit(int pc, int ir, int regs[]);
    void dumpRegisters(int regs[]);
    void dumpMemory(int mem[]);
    void instruction_cycle(void);

// CPU declarations -- note we're being horrible and using global variables; after we
// see structures and pointers, we'll clean things up.
//
#define NBR_REGS 10
#define MEMLEN 100
    // *** WILL BE ADDED TO IN LAB 9 ***
    int running;          // true iff instruction cycle is active
	int regs[NBR_REGS];   // general-purpose registers
	int mem[MEMLEN];      // main memory

	int pc;
	int ir;

// Main program: Initialize the cpu, read initial memory values,
// and execute the read-in program starting at location 00.
//
int main(void)
{
	printf("Michael Orlowski - Section 01 - Lab 9\n");
	initCPU();
	readMemory();
    dumpMemory(mem);
	char prompt[] = "> ";

	printf("\nBeginning execution:\n");
	printf("At the %sprompt, press return to execute the next instruction\n", prompt);
	printf("(or d to dump registers and memory, q to quit, or h or ? for help)\n");

	char command[80];
	fgets(command, sizeof command, stdin); /// for some reason it wants to read in an extra buffer, this stops that
	int commands_done = 0;                 /// true iff simulator quit command seen
    while (commands_done==0)
    {
        printf("%s", prompt);
        fgets(command, sizeof command, stdin);   // Read through end of current line.
        //printf("Command entered is: %s \n", command);
        if(strcmp(command, "q\n") == 0)
        {
            commands_done = 1;
        }
        else if (strcmp(command, "d\n") == 0)
            dumpControlUnit(pc, ir, regs);
        else if ((strcmp(command, "h\n") == 0) || (strcmp(command, "?\n") == 0))
            helpMsg();
        else if ((strcmp(command, "\n") == 0))
        {
            instruction_cycle();
        }
        else
            printf("Fuck off.");
    }

}

// Print out the instruction set architecture for the SDC.
//
void helpMsg(void)
{
	//print the instruction set
     printf("\nInstruction set:\n");
     printf("0xxx: HALT\n");
     printf("1RMM: Load R <- M[MM]\n");
     printf("2RMM: Store M[MM] <- R\n");
     printf("3RMM: Add M[MM] to R\n");
     printf("4Rxx: Negate R\n");
     printf("5RMM: Load Immediate R <- MM\n");
     printf("6RMM: Add Immediate R <- R + MM\n");
     printf("7xMM: Branch to MM\n");
     printf("8RMM: Branch Positive to MM if R > 0;\n");
     printf("90xx: Read char into R0\n");
     printf("91xx: Print char in R0\n");
     printf("92MM: Print string starting at MM.\n");
     printf("93MM: Dump control unit.\n");
     printf("94MM: Dump memory.\n");
}


// Initialize cpu registers, memory, pc, and ir (all to zeros).
// Initialize the running flag to true.
//
void initCPU(void)
{
    pc = 0;
    ir = 0;
	printf("initCPU\n");
    running=1;
	int i;
	for (i = 0; i < NBR_REGS; i++)
		regs[i] = 0;
	for (i = 0; i < MEMLEN; i++)
		mem[i] = 0;
}

// Read and dump initial values for memory
//
void readMemory(void)
 {
    int boolean = 1;
	int loc = 0;
	printf("Read memory: At the prompt, enter the value for the indicated\n");
	printf("memory address.  Enter a number > 9999 or < -9999 when you're done.\n");
	int buffer =0;
	while (boolean == 1)
    {
        scanf("%d", &buffer);
        if(buffer>-10000 && buffer<10000&&loc<MEMLEN)
        {
            mem[loc]=buffer;
        }
        else
            return;
        loc++;
	}

	printf("readMemory\n");


// *** You might need this after reading the sentinel ***
//	fgets(skip, sizeof skip, stdin);   // Clear the \n at end of terminating line

	printf("\nInitial value of memory:\n");
	dumpMemory(mem);
}


// dumpControlUnit(/*pc,ir,*/regs): Dump the control unit (pc, ir, and data registers).
// [For lab 8, just the registers]
//
void dumpControlUnit(int pc, int ir, int regs[])
{
    printf("dumpControlUnit\n\n");
	printf("PC: ");
	if(pc>=-999)
        printf(" ");
    printf("%d  IR: ", pc);
    if(ir>=-999)
        printf(" ");
    printf("%d\n\n", ir);
	dumpRegisters(regs);
	printf("\n\n");
	dumpMemory(mem);
}


// dumpRegisters(regs): Print register values in two rows of five.
//
void dumpRegisters(int regs[])
{
	// *** STUB *** Hint: How do printf formats %d, %5d, %-5d, %05d, and % 05d differ?
	printf("dumpRegisters\n\n");

	int i = 0;
    int j = 0;
    int rCount=0;
  for (i = 0; i < 2; i++)
    {
      if(regs[i * 10]>=-999) //if not a 5 length number
        printf(" ");
      for (j = 0; j < 5; j++)
	{
	  printf("R%d: %04d ", rCount, regs[rCount]);
	  rCount++;
	  if(regs[i * 10+j+1]>=-999)
        printf(" ");
	}
      printf("\n");

    }

}

// dumpMemory(mem): Print memory values in a table, ten per row,
// with a space between each group of five columns and with a header
// column of memory locations.
//
void dumpMemory(int mem[])
{
	printf("dumpMemory\n\n");
	int i = 0;
    int j = 0;

        for (i = 0; i < 10; i++)
    {
          printf("%3d: ", i * 10);
          if(mem[i * 10]>=-999) //if not a 5 length number
            printf(" ");
          for (j = 0; j < 10; j++)
        {
          printf("%04d ", mem[i * 10 + j]);
          if(mem[i * 10+j+1]>=-999)
            printf(" ");
          if (j == 4)
            printf("   ");
        }
          printf("\n");

    }
}


// Execute one instruction cycle.
//
int call_nbr = 0;
#define min(a, b) (((a) < (b)) ? (a) : (b))
void instruction_cycle(void)
 {
     char instruction[4]; // instruction is the 4 digit code in memory and turning it into an array of characters allows us to parse it accordingly
     ir = mem[pc];
     sprintf(instruction, "%d", ir); //typecasting the int to a string to get us our desired array
     int r = instruction[1]-'0'; // gets us  0,1,2,3,4,5,6,7,8, or 9 if those characters appear at the position to get us our register number
     int s = (instruction[2]-'0')*10 + (instruction[3] - '0'); //same as above but instead gets us our memory location number

    if(running==1)
    {
        if(pc>=MEMLEN||pc<0)
        {
            printf("Illegal pc: %d; halting\n", pc);
            return;
        }
        printf("At %d instruction %c %c %c%c: ", pc, instruction[0], instruction[1], instruction[2], instruction[3] );
        //Now that the generic print is accomplished, process instructions
        if(instruction[0]== '0')
        {
            printf("halting;\n");
            running=0;
        }
        else if(instruction[0]== '1')
        {
            printf("LD\tR%c <- M[%c%c] = %d", instruction[1], instruction[2], instruction[3], mem[s]);
            regs[r] = mem[s];
        }
        else if(instruction[0]== '2')
        {
            printf("ST\tM[%c%c] <- R%c = %d", instruction[2], instruction[3], instruction[1], regs[r]);
            mem[s] = regs[r];
	    }
        else if(instruction[0]== '3')
        {
            printf("ADD\tR%c <- R%c + M[%c%c] = %d + %d = %d" ,instruction[1], instruction[1], instruction[2], instruction[3], regs[r], mem[s],
            mem[s] + regs[r]);
            regs[r] = mem[s];
        }
        else if(instruction[0]== '4')
        {
            printf("NEG\tR%c <- -R%c = %d", instruction[1], instruction[1], (-1 * regs[r]));
            regs[r] = (-1 * regs[r]);
        }
        else if(instruction[0]== '5')
        {
            printf("LDM\tR%c <- %d", instruction[1], s);
            regs[r] = s;
        }
        else if(instruction[0]== '6')
        {
            printf("ADM\tR%c <- R%c + %d", instruction[1], instruction[1], s + regs[r]);
            regs[r] = s + regs[r];
        }
        else if(instruction[0]== '7')
        {
            printf("BR\t%d", s);
            pc = s - 1;
        }
        else if(instruction[0]== '8')
        {
            printf("BRP\t%d if R%c = %d > 0", s, instruction[1], regs[r]);
            if (regs[r] > 0)
            pc = s - 1;
        }

	    else if(instruction[0]== '9')
        {
            char input1;
            if (instruction[1]== '0')
            {
                printf("Please enter an integer character: ");
                scanf("%c", &input1);
                regs[0] = (int)input1;
            }
            else if (instruction[1]== '1')
            {
                printf("R0 is: %c", (char)regs[0]);//WE'LL DO THIS LATER
            }
            else if (instruction[1]== '2')
            {
                //use a while-loop
                int temporarypc = s;
                printf("Print string:  ");

                while(!(mem[temporarypc] == 0))
                {
                    printf("%c", (char) mem[temporarypc]);
                    temporarypc++;
                }
                printf("\n");

            }
            else if (instruction[1]== '3')
            {
                dumpControlUnit(pc, ir, regs);
            }
            else if (instruction[1]== '4')
            {
                dumpMemory(mem);
            }

        }
            pc++;
    }
    else
    {
        printf("Not running\n");
    }
	return;



}

