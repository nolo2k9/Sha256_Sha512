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

struct Point
{
    int x;
    int y;
};

/*
A union is similar to a struct i.e a class with member variables.
All of the member variables operate the same chunk of memory.
Both bytes and words take up the sme 512 bytes of memory.
*/
union Block
{
    BYTE bytes[64];
    WORD words[16];
    u_int64_t sixf[8];
};

enum Status
{
    READ,
    PAD,
    END
};
//Get the next block
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits)
{
    //Unsigned long(Number of bytes read)
    size_t nobytes;
    if (*S == END)
    {
        return 0;
    }
    else if (*S == READ)
    {
        //Try to read 64 bytes.
        nobytes = fread(B->bytes, 1, 64, f);
        //Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);
        // Enough room for padding
        if (nobytes == 64)
        {
            return 1;
        }
        else if (nobytes <= 55)
        {
            //Append a 1 bit (and 7 0 bits to make a full byte)
            B->bytes[nobytes++] = 0x80; //In bits 10000000
            // Append enough 0 bits, leaving 64 at the end.
            while (nobytes++ < 56)
            {
                B->bytes[nobytes] = 0x00; //In bits: 00000000
            }
            //Append length of original input(CHECK ENDIANS).
            B->sixf[7] = *nobits;
            // Say this is the last block.
            *S = END;
        }
        else
        {
            /* 
                Gotten to the end of the input message 
                Not enough room in this block for all the padding
                Appened a 1 bit and 7 0 bits to make a full byte
            */
            B->bytes[nobytes] = 0x80;
            //Appened 0 bits.
            while (nobytes++ < 64)
            {
                B->bytes[nobytes] = 0x00; //In bits: 00000000
            }
            //Change the status to END.
            *S = END;
        }
    } else if (*S == PAD)
    {
        nobytes =0;
        //Appened 0 bits.
            while (nobytes++ < 56)
            {
                B->bytes[nobytes] = 0x00; //In bits: 00000000
            }
            //Append nobits as an integer
            B->sixf[7] = *nobits;
            //Change the status to PAD.
            *S = PAD;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    //instance of union/ current block
    union Block B;

    //Total number of bits read.
    uint64_t nobits = 0;

    //current status of reading input
    enum Status S = READ;

    // File pointer for reading.
    FILE *f;

    // open file from command line
    f = fopen(argv[1], "r");

    //Loop through the (preprocessed) blocks from the input
    while (next_block(f, &B, &S, &nobits))
    {
        //Print the 16 32 bit words to the screen.
        for (int i = 0; i < 16; i++)
        {
            printf("%08" PF " ", B.words[i]);
        }
        printf("\n");
    }
    //close the file
    fclose(f);
    //print the total number of bits
    printf("Total bits read: %d. \n", nobits);

    return 0;
}