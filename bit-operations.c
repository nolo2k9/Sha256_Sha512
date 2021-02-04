#include <stdio.h>

void bin_print(int i){
    // number of bits in an integer
    int j = sizeof(int) * 8;
    int k;

    for(j-- ; j >=0; j--){

        k = ((10 << j)& i) ? 1 : 0;
        printf("%d", k );


    }
}

int main(int argc, char *argv[])
{
    int i =241;
    printf("Original:  ");
    bin_print(i);
    printf("\n");

    for(int j = 0; j < 40; j++){
        printf("%3d << %2d: ", i, j);
        bin_print(i << j);
        printf("\n");
    }

    return 0;
}