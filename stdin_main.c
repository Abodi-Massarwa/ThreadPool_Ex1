#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
///////////////////////////////////
///// INITS //////
#define LIST_SIZE 6
#define MAX_SIZE 1024
//pthread_t thread_list[LIST_SIZE];

struct my_thread{
    int m_id; // TODO assuming it helps us in the matter or putting back data in order after the multi-threading is done.
    int m_start_index; /// since we are dealing with char* tasks can be divided by simply indexes
    int m_end_index;
    int m_is_active; /// 1 active 0 not active
    pthread_t m_thread;
};

typedef struct my_thread my_thread;

my_thread thread_list[LIST_SIZE]; // our array of threads with a fixed size of 6

char our_string[MAX_SIZE]="\0";
char result_string[MAX_SIZE]="\0";
int our_key;

void print_mythread_info(my_thread thread)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\nthread_list[%d]",thread.m_id);
    printf("\nstart index=%d , end index=%d",thread.m_start_index,thread.m_end_index);
    printf("\nis active ?");
    if(thread.m_is_active==1)
    printf(" True");
    else /// thats extra we dont even receive dead threads in this function.
        printf(" False");
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

////// functions //////
void* thread_encrypt_function(void* thread)
{
    my_thread current_thread= *((my_thread*) thread);
    print_mythread_info(current_thread);
    size_t length= current_thread.m_end_index-current_thread.m_start_index+1;
    char current_sub_string[length];
    /// now lets get to real work and call the desired function
    strncpy(current_sub_string, our_string + current_thread.m_start_index, length);
    current_sub_string[length] = '\0';  // Add null terminator
    encrypt(current_sub_string,our_key);
    /// now append this to the global result string
    strcat(result_string, current_sub_string);



}



void* thread_decrypt_function(void* thread)
{
    my_thread current_thread= *((my_thread*) thread);
    print_mythread_info(current_thread);
}




void start_multithreading(char indicator, int key, char* data, int char_count)
{
    /// lets divide tasks for each thread based on how many chars we have
    char_count= strlen(data);
    int remainder=0;
    int chunk_size = char_count / LIST_SIZE;
    int current_char_number=char_count;
    int index_counter=0;
    if(char_count%LIST_SIZE!=0)
        remainder= char_count%LIST_SIZE;
    for (int i = 0; i < LIST_SIZE; ++i)
    {
        if(current_char_number!=0) {
            thread_list[i].m_is_active=1;
            thread_list[i].m_start_index=index_counter;
            index_counter=index_counter+chunk_size;
            thread_list[i].m_end_index=index_counter-1;
            current_char_number-=chunk_size;
        }
     if(i==0) {
         thread_list[i].m_end_index += remainder;
         index_counter += remainder;
         current_char_number-=remainder;
     }
    }
    //// done indexes divided to each thread;
    for (int i = 0; i < LIST_SIZE; ++i) {
        printf("thread[%d] : start index=%d ended index=%d \n",i,thread_list[i].m_start_index,thread_list[i].m_end_index);
        if(thread_list[i].m_is_active==1)
            printf("is active\n");
        else
            printf("is not\n");
    }
/*
 * TODO based on the char ('e' | 'd') we know whether its Encrypt or Decrypt to call
 * /// so now our thread_list is ready
 */
    if (indicator=='e')
    {
        for (int i = 0; i < LIST_SIZE; ++i) {

            my_thread current_thread = thread_list[i];

            if (current_thread.m_is_active == 1)/// means its active

            {


                pthread_create(&(thread_list[i].m_thread), NULL, thread_encrypt_function, &thread_list[i]);

            }

        }
    }
    else /// indicator='d'
    {
        for (int i = 0; i < LIST_SIZE; ++i) {

            my_thread current_thread = thread_list[i];

            if (current_thread.m_is_active == 1)/// means its active

            {

                pthread_create(&(thread_list[i].m_thread), NULL, thread_decrypt_function, &thread_list[i]);

            }

        }
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
        strcpy(our_string,lastData);
        our_key=key;
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

        printf("\nTHE GLOBAL RESULT STRING IS %s\n",result_string);
        printf("the last char is %c",result_string[0]);
	}

	return 0;
}
