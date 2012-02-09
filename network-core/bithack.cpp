#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
using namespace std;

template <typename T>
static inline char GB(const T x, const char s, const char n)
{
	return (x >> s) & (((T)1U << n) - 1);
}

void prnmem(char* addr, unsigned int size){
   write(1, addr, size);
   printf("\n");
}

struct Packet {
   char id;
   char space[3];
   unsigned int sdata;
   int data[4];
   Packet() { 
      id = 61;
      space[0] = 0x20;
      space[1] = 0x20;
      space[2] = 0x20;
      sdata = 0x4820454c;
      data[0] = 0x20202061;
      data[1] = 0x20202062;
      data[2] = 0x20202063;
      data[3] = 0x20202064;
   }
};

int main () {
   // environment variables

   printf("char: %d\n", sizeof(unsigned char));
   printf("short: %d\n", sizeof(unsigned short));
   printf("short int: %d\n", sizeof(unsigned short int));
   printf("int: %d\n", sizeof(unsigned int));
   printf("long: %d\n", sizeof(unsigned long));
   printf("long int: %d\n", sizeof(unsigned long int));
   printf("long long: %d\n", sizeof(unsigned long long));
   printf("long long int: %d\n", sizeof(unsigned long long int));

   unsigned int z = 1; /* 0x00000001 */
   printf("%s\n", *((unsigned char *) &z) == 0 ? "big-endian" : "little-endian");

   char buffer[128];
   unsigned int var = 0x4820454C;

   // method 1 - char pointer

   unsigned int* hck = (unsigned int*)buffer;
   *hck = var;
   prnmem(buffer, sizeof(int));

   // method 2 - bitwise function
   
   buffer[0] = GB(var, 0, 8);
   buffer[1] = GB(var, 8, 8);
   buffer[2] = GB(var, 16, 8);
   buffer[3] = GB(var, 24, 8);
   prnmem(buffer, sizeof(int));  

   // method 3 - union
   
   union {
      unsigned int d;
      char f;
   };
 
   d = 0x4820454C;
   prnmem(&f, sizeof(int));

   // accessing struct, 2 methods

   int i;
   Packet npck;
   Packet* pck_ptr = &npck;
   char* pck_hck_ptr = (char*) pck_ptr;
   
   prnmem((char *) &npck, sizeof(Packet));
   
   for (i = 0; i < sizeof(Packet); i++){
      buffer[i] = *(pck_hck_ptr+i);
   }
   prnmem(buffer, sizeof(Packet));


   return 0;
}

