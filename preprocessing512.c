#include <stdio.h>
#include <inttypes.h>

#define W 64
#define WORD uint64_t
#define PF PRIX64
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
Both bytes and words take up the sme 1024 bits of memory.
Sha512 works on blocks of 1024 bits
*/
union Block
{
    // 8 * 128 = 1024- dealing with block as bytes
    BYTE bytes[128];
    // 16 * 64 = 1024- dealing with block as words
    WORD words[64];
    // 64 * 16 = 1024 - dealing with the last 128 bits of the last block.
    u_int64_t sixf[16];
};

// For keeping track of where we are with the input message/padding.
enum Status
{
    READ,
    PAD,
    END
};

// Returns 1 if it created a new block - either original message or padding.
// Returns 0 if all padded message has already been consumed
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
        //Try to read 128 bytes from the input file.
        nobytes = fread(B->bytes, 1, 128, f);
        //Calculate the total bits read so far.
        *nobits = *nobits + (16 * nobytes);
        // Enough room for padding
        if (nobytes == 128)
        {
            //This happens when we can read 64 more bytes from f
            return 1;
        }
        else if (nobytes <= 121)
        {
            // This happens when we have enough room for all the padding.
            // Append a 1 bit (and 7 0 bits to make a full byte)
            B->bytes[nobytes++] = 0x80; //In bits 10000000

            // Append enough 0 bits, leaving 64 at the end.
            while (nobytes++ < 122)
            {
                B->bytes[nobytes] = 0x00; //In bits: 00000000
            }

            // Append length of original input(CHECK ENDIANS).
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
            while (nobytes++ < 122)
            {
                B->bytes[nobytes] = 0x00; //In bits: 00000000
            }
            //Append nobits as an integer
            B->sixf[7] = *nobits;
            //Change the status to PAD.
            *S = PAD;
    }

    return 1;
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

    return 1;
}