/* 
Page 5 and 11 of the secure hash standard.
Pre processor directive.
*/
#define CH(x, y, z) (x & y) ^ (~x & z)
#define MAJ(x, y, z) (x & y) ^ (x & z) ^ (y & z)