#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Michael Orlowski - Lab 3 - Section 01
int main()
{
    char s[100];
    printf("Michael Orlowski - Lab 3 - Section 01\n");
    do
    {
        int strln=0;
        int bleh=1;//boolean value
        while(bleh==1)
        {
            printf("Please enter a bit string or press 'q' to quit: ");
            scanf("%s", s);

            if (strcmp(s, "q") == 0)
            {
                printf("Thank you, good bye.");
                return 0;
            }
            while (s[strln] != '\0') //Code below finds length of string
            {
                if(!(s[strln]=='0' || s[strln]=='1'))
                {
                    strln = 0;
                    bleh = 1;
                    printf("You typed something other than a 0 or 1. Try again.\n");
                    break;
                }
                strln++;
                bleh = 0; //to exit outer while loop
            }
        }

        int val = 0;
		/////////////////positive find Block
		int pos=strln-1;
		if(s[0]=='0')
		{
			while(pos>=0)
			{
				if(s[pos]=='1')
					val+=pow(2,(strln-1-pos));
				pos--;
			}
		}
		else ////////////////////Negative find block
		{
			while(pos>=0)
			{
			if(s[pos]=='1')
				s[pos]='0';
			else
				s[pos]='1';
			pos--;
			}


			///////////////////////////add block
			pos=strln-1;
			int done=0;
			while(done==0 && pos>=0)
			{
				if(s[pos]=='0')
				{
					s[pos]='1';
					done=1;
				}
				else
				{
					s[pos]='0';
				}
				pos--;
			}

			//now that the string is converted to a positive number, get the value
			pos=strln-1;
			while(pos>=0)
			{
			    if(s[pos]=='1')
                {

                    val+=pow(2,(strln-1-pos));
                }
				pos--;
			}
			val*=-1;
		}
		///////////////////////////////////////////////////////print
		printf("The value of the bitstring is %d\n", val);
		//now compute the negative of the bitstring;




		if(val>0)
		{
			pos=strln-1;
			while(pos>=0)
			{
			if(s[pos]=='1')
				s[pos]='0';
			else
				s[pos]='1';
			pos--;
			}


			///////////////////////////add block
			pos=strln-1;
			int done=0;
			while(done==0 && pos>=0)
			{
			if(s[pos]==0)
				{
					s[pos]='1';
					done==1;
				}
				else
				{
					s[pos]='0';
				}
				pos--;
			}
		}
		printf("The negative of the bitstring is %s\n", s);





	}
	while(s[0]=='0'||s[0]=='1');

}
