#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
///////////////////////////////////
///// INITS //////
#define LIST_SIZE 6
//pthread_t thread_list[LIST_SIZE];

struct my_thread{
    int m_id; // TODO assuming it helps us in the matter or putting back data in order after the multi-threading is done.
    int m_start_index; /// since we are dealing with char* tasks can be divided by simply indexes
    int m_end_index;
    pthread_t m_thread;
};

typedef struct my_thread my_thread;

my_thread thread_list[LIST_SIZE]; // our array of threads with a fixed size of 6

////// functions //////
void start_multithreading(char indicator, int key, char* data, int char_count)
{
    /// lets divide tasks for each thread based on how many chars we have
    char_count= strlen(data);
    int remainder=0;
    int thread_portion = char_count/LIST_SIZE;
    int index_counter=0;
    if(char_count%LIST_SIZE!=0)
        remainder= char_count%LIST_SIZE;
    for (int i = 0; i < LIST_SIZE; ++i)
    {
     thread_list[i].m_start_index=index_counter;
     index_counter=index_counter+thread_portion;
     thread_list[i].m_end_index=index_counter-1;
     if(i==0) {
         thread_list[i].m_end_index += remainder;
         index_counter += remainder;
     }
    }
    //// done indexes divided to each thread;
    












/*
 * TODO based on the char ('e' | 'd') we know whether its Encrypt or Decrypt to call
 */
    if (indicator=='e')
    {

    }
    else /// indicator='d'
    {

    }










}



























/////////////////////////////////////
int main(int argc, char *argv[])
{
    for (int i = 0; i < 6; ++i) {
        /*
         *TODO assigning id to each thread to override the auto-init trash values
         * TODO regarding threads themselves tbh no one cares what values they hold . what matters the most
         * TODO is to get them to work
         */
       thread_list[i].m_id=i;
    }
    /*
     * get the threads to work using the relevant functions
     * TODO 1) pthread_t_create
     * TODO 2) pthread_t_join
     * TODO etc....
     */
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
        lastData[counter-1]='\0';
        //printf("lastdata[counter]=%c",lastData[counter]);
        char* action = argv[2];
        if(action[1]=='e') {
            printf("\nour string is :\n%s\n",lastData);
            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('e',key,lastData,counter);
            encrypt(lastData, key);
            printf("Encrypted data:\n%s\n",lastData);
//            decrypt(lastData,key);
//            printf("Decrypted data:\n%s\n",lastData);
        }
        else {// TODO "-d" as for Decrypt
            printf("\nour string is :\n%s\n",lastData);
            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('d',key,lastData,counter);
            decrypt(lastData, key);
            printf("Decrypted data:\n%s\n",lastData);
        }
        data[counter]='\0'; // TODO to make sure the string is printed without extra garbage letters
//        printf("original data:\n %s\n",data);
//		printf("Encrypted data:\n %s\n",lastData);
	}

	return 0;
}
