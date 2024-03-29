#include <stdio.h>
#include <inttypes.h>

#ifdef _MSC_VER

#include <stdlib.h>
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else

#include <byteswap.h>

#endif

const int _i = 1;
#define islilend() ((*(char *)&_i) != 0)

#define WORD uint64_t
#define PF PRIx64
#define BYTE uint8_t

// Page 5 of the secure hash standard.
#define ROTL(_x, _n) ((_x << _n) | (_x >> ((sizeof(_x) * 8) - _n)))
#define ROTR(_x, _n) ((_x >> _n) | (_x << ((sizeof(_x) * 8) - _n)))
#define SHR(_x, _n) (_x >> _n)

// Page 10 of the secure hash standard.
#define CH(_x, _y, _z) ((_x & _y) ^ (~_x & _z))
#define MAJ(_x, _y, _z) ((_x & _y) ^ (_x & _z) ^ (_y & _z))

//Page 10 of the secure hash standard
#define SIG0(_x) (ROTR(_x, 28) ^ ROTR(_x, 34) ^ ROTR(_x, 39))
#define SIG1(_x) (ROTR(_x, 14) ^ ROTR(_x, 18) ^ ROTR(_x, 41))
#define Sig0(_x) (ROTR(_x, 1) ^ ROTR(_x, 8) ^ SHR(_x, 7))
#define Sig1(_x) (ROTR(_x, 19) ^ ROTR(_x, 61) ^ SHR(_x, 6))

/*
A union is similar to a struct i.e a class with member variables.
All of the member variables operate the same chunk of memor_y.
Both b_ytes and words take up the sme 1024 bits of memor_y.
Sha512 works on blocks of 1024 bits
*/
union Block
{
    // 8 * 128 = 1024- dealing with block as bytes
    BYTE bytes[128];
    // 16 * 64 = 1024- dealing with block as words
    WORD words[64];
    // 64 * 16 = 1024 - dealing with the last 128 bits of the last block.
    uint64_t sixf[16];
};

// For keeping track of where we are with the input message/padding.
enum Status
{
    READ,
    PAD,
    END
};

/*
Section 4.2.3 of secure hash standard
SHA-384, SHA-512, SHA-512/224 and SHA-512/256 use the same sequence of eight_y constant { 5 1 2} { 5 1 2} { 5 1 2}
64-bit words, K0 , K1 ,, K79 . 
These words represent the first sixt_y-four bits of the fractional parts of the cube roots of the first eight_y prime numbers. 
In hex, these constant words are (from left to right)
*/
const WORD K[] = {

    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
    0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
    0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
    0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
    0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
    0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
    0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
    0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
    0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
    0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
    0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

// Returns 1 if it created a new block - either original message or padding.
// Returns 0 if all padded message has alread_y been consumed
int next_block(FILE *f, union Block *M, enum Status *S, uint64_t *nobits)
{
    //Unsigned long(Number of b_ytes read)
    size_t nobytes;
    if (*S == END)
    {
        return 0;
    }
    else if (*S == READ)
    {
        //Try to read 128 bytes from the input file.
        nobytes = fread(M->bytes, 1, 128, f);
        //Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);
        // Enough room for padding
        if (nobytes == 128)
        {
            //This happens when we can read 64 more bytes from f
            //return 1;
        }
        else if (nobytes < 120)
        {
            // This happens when we have enough room for all the padding.
            // Append a 1 bit (and 7 0 bits to make a full byte)
            M->bytes[nobytes] = 0x80; //In bits 10000000

            // Append enough 0 bits, leaving 64 at the end.
           for (nobytes++; nobytes < 120; nobytes++)
            {
                M->bytes[nobytes] = 0x00; //In bits: 00000000
            }

            // Append nobits as a big endian integer.
            M->sixf[15] = (islilend() ? bswap_64(*nobits) : *nobits);

            // Say this is the last block.
            *S = END;
        }
        else
        {
            /* 
                Gotten to the end of the input message 
                Not enough room in this block for all the padding
                Appened a 1 bit and 7 0 bits to make a full b_yte
            */
            M->bytes[nobytes] = 0x80;
            // Append 0 bits.
            for (nobytes++; nobytes < 128; nobytes++)
            {
                // Error: trying to write to
                M->bytes[nobytes] = 0x00; // In bits: 00000000
            }
            // Change the status to PAD.
            *S = PAD;
        }
    }
    else if (*S == PAD)
    {

        // Append 0 bits.
        for (nobytes++; nobytes < 120; nobytes++)
        {
            // Error: trying to write to
            M->bytes[nobytes] = 0x00; // In bits: 00000000
        }

        // Append nobits as a big endian integer.
        M->sixf[15] = (islilend() ? bswap_64(*nobits) : *nobits);
        *S = PAD;
    }
    // Swap the byte order of the words if we're little endian.
    if (islilend())
        for (int i = 0; i < 16; i++)
            M->words[i] = bswap_64(M->words[i]);

    return 1;
}

int next_hash(union Block *M, WORD H[])
{

    // Message schedule, Section 6.4.2
    WORD W[80];
    // Iterator.
    int t;
    // Temporar_y variables.
    WORD a, b, c, d, e, f, g, h, T1, T2;

    // Section 6.4.2, part 1.
    for (t = 0; t < 16; t++)
        W[t] = M->words[t];
    for (t = 16; t < 80; t++)
        W[t] = Sig1(W[t - 2]) + W[t - 7] + Sig0(W[t - 15]) + W[t - 16];

    // Section 6.4.2, part 2.
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    // Section 6.4.2, part 3.
    for (t = 0; t < 80; t++)
    {
        T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    // Section 6.4.2, part 4.
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];

    return 0;
}
// The function that performs/orchestrates the SHA256 algorithm on message f.
int sha512(FILE *f, WORD H[])
{

    // The current block.
    union Block M;

    // Total number of bits read.
    uint64_t nobits = 0;

    // Current status of reading input.
    enum Status S = READ;

    // Loop through the (preprocessed) blocks.
    while (next_block(f, &M, &S, &nobits))
    {
        next_hash(&M, H);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* 
    Section, 5.3.5 of the secure has standard
    For SHA-512, the initial hash value, H , shall consist of the following eight 64-bit words, in
    hex:
    */
    WORD H[] = {
        0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
        0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
        0x510e527fade682d1, 0x9b05688c2b3e6c1f,
        0x1f83d9abfb41bd6b, 0x5be0cd19137e2179

    };

    if (argc != 2) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf("usage: %s filename", argv[0]);
    }
    else
    {
        // File pointer for reading.
        FILE *file = fopen(argv[1], "r");

        /* fopen returns 0, the NULL pointer, on failure */
        if (file == 0)
        {
            printf("Could not open file\n");
        }
        else
        {
            int x;
            /* read one character at a time from file, stopping at EOF, which
               indicates the end of the file.  Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */
            sha512(file, H);

            // Print the final SHA512 hash.
            for (int i = 0; i < 8; i++)
                printf("%016" PF, H[i]);
            printf("\n");
            fclose(file);
        }
    }

    return 0;
}