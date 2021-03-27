#include <stdio.h>

void bin_print(unsigned int i){
    // number of bits in an integer
    int j = sizeof(unsigned int) * 8;

    int k;
    //loop ovthe number of bits in i, left to right
    for(j-- ; j >=0; j--){
        // if j is greater than 10 return 1 if false return 0
        // pick out the j-th bit of i 
        k = ((1 << j)& i) ? 1 : 0;
        //print k
        printf("%d", k );


    }
}

int main(int argc, char *argv[])
{
    for(int i = 0;  i < argc; i ++)
    {
        printf("%s\n", argv[i]);
    }
    //printf("%d\n", argc);
    //return 0;

    printf("\n");
    //set i to a literal value
    unsigned int i =0x0f0f0f0f;
    printf("Original: \t  ");
    //print i in binary
    bin_print(i);
    //endline
    printf("\t%x\t%u\n", i, i);

    //
   int j= (sizeof(unsigned int) * 8);

    for(j--; j >=0; j--){
        // 1 shifted left j times
        bin_print(1 << j);printf("\n");
        // i 
        bin_print(i);printf("\n");
        
        printf("-------------------------------- &\n");
        // 1 shifted left j times bitwise logical and i
        bin_print(((1 << j) & i)); printf("\n\n");
    }
    //everything ok
    return 0;
}