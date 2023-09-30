#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{

    FILE *fptr;
    fptr = fopen(*(&argv[1]), "r");
    if(fptr == NULL) {
        printf("File not openned!");
    }

    // CURRENT GAME STATE
    // INIT ALL VALUES WITH -2
    int state[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[i][j] = -2;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%5d\t", state[i][j]); // Dont forget to set two tabs in the printings \t\t
        }
        printf("\n");
    }
    printf("\n");

/* 
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%5d\t", answer[i][j]); // Dont forget to set two tabs in the printings \t\t
        }
        printf("\n");
    }
 */
/* 
    for (int i = 0; i < 4; i++)
    {
        fscanf(fptr, "%d", )
    }
     */

/* 
    while(fscanf("%s,%s,%s,%d", sem, type, title, &value) == 4) {
    //do something with the read line values
    } */
    //int buffer;
    int answer[4][4];/* = { {1, 2, -1, 1},
                         {1, -1, 2, 1},
                         {1, 2, 1, 1},
                         {0, 1, -1, 1}, }; */
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            fscanf(fptr,"%i,", &answer[i][j]/* buffer */);
            //answer[i][j] = buffer;
        }
    }

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%5d\t", answer[i][j]);
        }
        printf("\n");
    }


    return 0;
}