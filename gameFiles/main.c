#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct action {
    int type;
    int coordinates[2];
    int board[4][4];
};


int main(int argc, char **argv)
{

    FILE *fptr;
    fptr = fopen(*(&argv[1]), "r");
    if (fptr == NULL)
    {
        printf("File not openned!");
    }

    int answer[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fscanf(fptr, "%i,", &answer[i][j]);
        }
    }
/* 
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%5d\t", answer[i][j]);
        }
        printf("\n");
    }
    printf("\n");
 */

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

    return 0;
}