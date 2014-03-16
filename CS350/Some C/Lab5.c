#include <stdio.h>
#include <stdlib.h>
//Michael Orlowski - Section 01 - Lab 5
int main()
{
    printf("LAB 5 - MICHAEL ORLOWSKI - SECTION 01\n");
    unsigned int usersHex;
    int left, right;
    int boolean = 1;
     do//DO-WHILE FOR REPEATED USER INPUT
    {
        printf("\n");
        printf("Please enter X in hexadecimal or '0' to stop:\n");
        scanf(" %x", &usersHex);

        if (usersHex == 0)//ENDS PROGRAM IF 0 INPUTED
            {
                printf("Thank you, good bye.\n");
                return 0;
            }

         printf("Please enter the left endpoint: \n");
         scanf(" %d", &left);
         printf("Now the right: \n");
         scanf(" %d", &right);

            while (left < 0 || (right > 15 || right <= 0))//while to easily break out of if wrong input and get back into do-while
            {
                if (left < 0)//if statements to notify user of wrong input
                printf("%d is an illegal endpoint.\n", left);
                if (right > 15 || right <= 0)
                printf("%d is an illegal endpoint.\n", right);
                printf("Please try again.\n");
                break;
            }
        if (left >= 0 && (right <= 15 && right > 0))
        {
            unsigned int mask = 0xffff;   ///original was ffffffff
            mask = mask >> (left);
            usersHex = usersHex & mask;
            usersHex = usersHex >> (15 - right);
            printf("X[%d:%d] = 0x%x = %d", left, right, usersHex, usersHex);
        }



    } while (boolean == 1);

      return 0;
}


