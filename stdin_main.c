#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
///////////////////////////////////
#define LIST_SIZE 6
pthread_t thread_list[LIST_SIZE];

































/////////////////////////////////////
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
	    printf("usage: key -[e|d] < file \n");
	    printf("!! data more than 1024 char will be ignored !!\n");
	    return 0;
	}

	int key = atoi(argv[1]);
	printf("key is %i \n",key);

	char c;
	int counter = 0;
	int dest_size = 1024;
	char data[dest_size]; 
	

	while (((c = getchar()) != EOF) &&(counter != 1024))
	{
	  data[counter] = c;
	  counter++;

//	  if (counter == 1024){
//          break;
//		encrypt(data,key);
//		printf("Encrypted data: %s\n",data);
//		counter = 0;
//	  }
	}
	/*
	 * after we exit the loop we need to make sure whether the text reached the maximum length
	 * or simply because the file ended and the counter isn't 1024
	 */
	if (counter > 0)
	{

		char lastData[counter];
		lastData[0] = '\0';
		strncat(lastData, data, counter);
        lastData[counter]='\0';
        //printf("lastdata[counter]=%c",lastData[counter]);
        char* action = argv[2];
        if(action[1]=='e') {
            printf("\nour string is :\n%s\n",lastData);
            encrypt(lastData, key);
            printf("Encrypted data:\n%s\n",lastData);
//            decrypt(lastData,key);
//            printf("Decrypted data:\n%s\n",lastData);
        }
        else {// TODO "-d" as for Decrypt
            printf("\nour string is :\n%s\n",lastData);
            decrypt(lastData, key);
            printf("Decrypted data:\n%s\n",lastData);
        }
        data[counter]='\0'; // TODO to make sure the string is printed without extra garbage letters
//        printf("original data:\n %s\n",data);
//		printf("Encrypted data:\n %s\n",lastData);
	}

	return 0;
}
