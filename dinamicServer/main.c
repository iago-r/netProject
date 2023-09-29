/* #include <stdio.h>
#include <string.h>

int main () {
   char str1[15];
   char str2[15];
   int ret;


   strcpy(str1, "ABCDEFG");
   strcpy(str2, "ABCDEF");

   ret = strcmp(str1, str2);

   if(ret < 0) {
      printf("str1 is less than str2\n");
   } else if(ret > 0) {
      printf("str2 is less than str1\n");
   } else {
      printf("str1 is equal to str2\n");
   }
   
   return(0);
} */

//./a.out in.txt
//gcc main.txt

#include <stdio.h>
#include <stdlib.h>

enum type {start, reveal, flag, state, remove_flag, reset, win, exit, game_over};

struct action {
  int type;
  int coordinates[2];
  int board[4][4];
};


int main(int argc, char **argv)
{

  //FILE *fptr;
  //int buffer;
  // adaptar o numero dentro de argv para o parametro correto
  //fptr = fopen(*(&argv[1]), "r");
  /* k */
  // fscanf(fptr, "%s",&buffer);
  // printf("%i", buffer);

/*   while (fscanf(fptr, "%i", &buffer) == EOF)
  {
    printf("%i", buffer);
  }
 */

  int answer[4][4] = { {1, 2, -1, 1},
                       {1, -1, 2, 1}, 
                       {1, 2, 1, 1}, 
                       {0, 1, -1, 1}, };

  // init state com todas as células ocultas
  int state[4][4]; /* = { {-2, -2, -2, -2},
                      {-2, -2, -2, -2}, 
                      {-2, -2, -2, -2}, 
                      {-2, -2, -2, -2}, };
 */

  // Init State
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      state[i][j] = -2;
    }
  }

  // Print current state
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      printf("%5d\t\t", state[i][j]);
    }
    printf("\n");
  }


  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      printf("%5d\t\t", answer[i][j]);
    }
    printf("\n");
  }
  







  return 0;
}