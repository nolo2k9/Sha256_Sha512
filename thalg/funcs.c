#include <stdio.h>
#include <inttypes.h>

#define WORD uint32_t
#define PF PRIX32

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
{

    return (x & y) ^ (~x & z);
}

int main(int argc, char *argv[])
{

    uint32_t x = 0xf0f0f0f0;
    uint32_t y = 0x0f0f0f0f;
    uint32_t z = 0xf0f0f0f0;


    uint32_t ans = Ch(x,y,z);

    printf("Ch (%08"PRIX32",%08"PRIX32",%08"PRIX32") = %08" PRIX32 "\n", x,y,z, ans);

    return 0;
}