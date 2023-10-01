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
    // OPEN FILE
    {
        
    }
    FILE *fptr;
    
    if(argc < 2) {
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
            //printf("%5d\t", state[i][j]); // Dont forget to set two tabs in the printings \t\t
        }
        printf("\n");
    }
    printf("\n");

    char action[255];
    char actionTypes[9][12] = {"start", "reveal", "flag", "state",
                               "remove_flag", "reset", "win", "exit", 
                               "game_over"};
    int i; // declared outside to detect if the command is valid
    while(1)
    {
        //int ReadUserAction()
        scanf("%s", action);
        for (i = 0; i < 9; i++)
        {
            if(strcmp(action, actionTypes[i]) == 0) {
                // the action will be the index of the loop
                printf("[%s]: %i\n", action, i);
                break;
            }
        }
        printf("%i\n", i);

        // CLIENT
        switch (i)
        {
            // CLIENT....................START
            case 0:
                /* code */
                break;
            
            // CLIENT....................reveal [int],[int]
            case 1:
                /* code */
                break;
            
            // CLIENT....................flag [int],[int]
            case 2:
                /* code */
                break;
            
            // CLIENT....................remove_flag [int],[int]
            case 4:
                /* code */
                break;

            // CLIENT....................reset
            case 5:
                /* code */
                break;
            
            // CLIENT....................exit
            case 7:
                /* code */
                break;
                
            default:
                if(i==9){printf("Invalid action.\n"); return 0;}
                break;
        }

        switch (i)
        {
            // SERVER....................state
            case 3:
                /* code */
                break;

            // SERVER....................win
            case 6:
                /* code */
                break;
            
            // SERVER....................game_over
            case 8:
                /* code */
                break;
        }
            
            

    }



/*     while(1){
    } */












    return 0;
}