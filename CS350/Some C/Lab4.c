#include <stdio.h>
#include <stdlib.h>
//Michael Orlowski - Section 01 - Lab 4
int main()
{
    unsigned int usersHex;
    int left, right;
    printf("Please enter X in hexadecimal: ");
    scanf("%x/n", &usersHex);
    printf("Please enter the left and right endpoints: ");
    scanf("%d%d/n", &left, &right);

    unsigned int mask = 0xffffffff;
    //if (left > 0)
    //{
        mask = mask >> (left);
        usersHex = usersHex & mask;

    //}

        usersHex = usersHex >> (31 - right);
        printf("X[%d:%d] = 0x%X = %d", left, right, usersHex, usersHex);

    return 0;
}
