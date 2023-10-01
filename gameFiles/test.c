#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int type;
    int coordinates[2];
    int board[4][4];
} action ;

int main(void) {

    action acao1;
    acao1.type = 3;
    acao1.coordinates[0] = 7;
    acao1.coordinates[1] = 5;
    acao1.board[0][0] = 1;  acao1.board[0][0] = 2;  acao1.board[0][0] = -1; acao1.board[0][0] = 1;
    acao1.board[0][0] = 1;  acao1.board[0][0] = -1; acao1.board[0][0] = 2;  acao1.board[0][0] = 1;
    acao1.board[0][0] = 1;  acao1.board[0][0] = 2;  acao1.board[0][0] = 1;  acao1.board[0][0] = 1;
    acao1.board[0][0] = 0;  acao1.board[0][0] = 1;  acao1.board[0][0] = -1; acao1.board[0][0] = 1;
    
    char dest[3 * sizeof(int)];

    memcpy(dest, &acao1, 3*sizeof(int));

    action acao2;

    memcpy(&acao2, dest, 3*sizeof(int));

    printf("%d\n",acao2.type);
    printf("%d,%d\n",acao2.coordinates[0], acao2.coordinates[1]);


    printf("// OVERWRITING TEST..................................\n");
    acao2.coordinates[0]=3;
    acao2.coordinates[1]=2;
    printf("%d,%d\n",acao2.coordinates[0], acao2.coordinates[1]);
    memcpy(&acao2, dest, 3*sizeof(int));
    printf("%d,%d\n",acao2.coordinates[0], acao2.coordinates[1]);
    return 0;
}