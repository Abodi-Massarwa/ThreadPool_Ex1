#include "codec.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	char data[] = "Hello world";
	int key = 12;

    /* TODO:MY THOUGHTS
     * it appears both encrypt and decrypt functions modify the original data passed to them
     * as an array of chars which is equivalent to char*
     * flashback from C : you better not use the original pointer in the process of iterating since you
     * would change its address
     */
	encrypt(data,key);
	printf("encrypted data: %s\n",data);

	decrypt(data,key);
	printf("decrypted data: %s\n",data);

	return 0;
}
