#include <stdio.h>

int answer[4][4];

int main(int argc, char** argv) {
    FILE* fp;
    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("cant open %s\n", argv[1]);
        return 1;
    }
    for (int i = 0; i < 4; i++)
    {
        fscanf(fp, "%i,%i,%i,%i", &answer[i][0], &answer[i][1], &answer[i][2], &answer[i][3]);
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%i, ", answer[i][j]);
        }
        printf("\n");
    }
    
    
    return 0;
}