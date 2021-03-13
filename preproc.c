#include <stdio.h>
#include <inttypes.h>

#define W 32
#define WORD uint32_t
#define PF PRIX32
#define BYTE uint8_t

/*
A struct is similar to a class except it doesn't have a method associated with it.
Put variables that are related with each other together
*/

struct Point{
    int x;
    int y;
};

/*
A union is similar to a struct i.e a class with member variables.
All of the member variables operate the same chunk of memory.
Both bytes and words take up the sme 512 bytes of memory.
*/
union Block{
    BYTE bytes[64];
    WORD words[16];
};

int main(int argc, char *argv[]){
    //instance of union/ current block
    union Block B;
    //Total number of bits read.
    uint64_t nobits = 0;
    // File pointer for reading.
    FILE *f;
    // open file from command line
    f = fopen(argv[1], "r");

    //Unsigned long(Number of bytes read)
    size_t nobytes;
    //Try to read 64 bytes
    nobytes = fread(B.bytes, 1, 64, f);
    //Tell the command line ab how many has been read
    printf("Read %d bytes. \n", nobytes);
    //Update the number of bits read
    nobits = nobits + (8 * nobytes);
    //Print the 16 32 bit words to the screen.
    for (int i =0; i < 16; i ++)
    {
        printf("%08" PF " ", B.words[i]);
        if((i + 1) % 8 == 0)
            printf("\n");
    }
    while (!feof(f)){
        //Tell the command line ab how many has been read
        nobytes = fread(B.bytes, 1, 64, f);
        printf("Read %d bytes. \n", nobytes);
        //Update the number of bits read
        nobits = nobits + (8 * nobytes);
        //Print the 16 32 bit words to the screen.
        for (int i =0; i < 16; i ++)
        {
            printf("%08" PF " ", B.words[i]);
            if((i + 1) % 8 == 0)
                printf("\n");
        }    
    }
    //close the file
    fclose(f);
    //print the total number of bits
    printf("Total bits read: %d. \n", nobits);

    return 0;
}