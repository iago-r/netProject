#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct action
{
    int type;
    int coordinates[2];
    int board[4][4];
};

int main(int argc, char **argv)
{
    FILE *fptr;

    if (argc < 2)
    {
        printf("Type file name.\n");
        return 0;
    }

    fptr = fopen(*(&argv[1]), "r");
    
    if (fptr == NULL)
        printf("File not openned!");

    // LOAD ANSWER FROM EXTERN FILE
    int answer[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fscanf(fptr, "%i,", &answer[i][j]);
        }
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
            printf("%5d\t", answer[i][j]); // Dont forget to set two tabs in the printings \t\t
            // printf("%5d\t", state[i][j]); // Dont forget to set two tabs in the printings \t\t
        }
        printf("\n");
    }
    printf("\n");

    char buf[1024];
    struct action acao2;
    char actionTypes[9][12] = {"start", "reveal", "flag", "state",
                               "remove_flag", "reset", "win", "exit",
                               "game_over"};
    int i; // declared outside to detect if the command is valid
    while (1)
    {
        // SCANNING MESSAGE.........................................................................
        memcpy(&acao2, buf, 3 * sizeof(int)); // revisar se vou utilizar sizeof(int) ou sizeof(action) // não precisa de usar o scanf aqui, creio

        // CLIENT TYPE MESSAGES.....................................................................
        switch (acao2.type)
        {
        // start
        case 0:
            /* code */
            break;

        // reveal [int],[int]
        case 1:
            /* code */
            break;

        // flag [int],[int]
        case 2:
            /* code */
            break;

        // remove_flag [int],[int]
        case 4:
            /* code */
            break;

        // reset
        case 5:
            /* code */
            break;

        // exit
        case 7:
            /* code */
            break;

        default:
            printf("Invalid action.\n");
            return 0;
        }
    }

    return 0;
}