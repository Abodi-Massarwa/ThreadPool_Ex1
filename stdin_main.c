#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
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
    char m_string[(MAX_SIZE/LIST_SIZE)+1]; /// worst case scenario each thread holds 1024/6 chars
    pthread_t m_thread;
};

typedef struct my_thread my_thread;

my_thread thread_list[LIST_SIZE]; // our array of threads with a fixed size of 6

char our_string[MAX_SIZE]={0};
char result_string[MAX_SIZE]={0};
int our_key;

void print_mythread_info(my_thread thread)
{
    //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>");
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
    /*
     *  i just realized we need to give each thread its own char[] to prevent shared variables and race conditions
     *  and to not use mutex which is gonna be costy
     *  TODO 1) lets give each thread its own char[]
     */
    my_thread* current_thread_ptr=((my_thread*)thread);
    my_thread current_thread= *((my_thread*) thread);
    print_mythread_info(current_thread);
    size_t length= current_thread.m_end_index-current_thread.m_start_index+1;
    char current_sub_string[length];
    current_sub_string[length-1]='\0';
    /// now lets get to real work and call the desired function
    strncpy(current_sub_string, our_string + current_thread.m_start_index, length);
    current_sub_string[length] = '\0';  // Add null terminator
    printf("\nTHREAD[%d] substring before encryption= %s",current_thread.m_id,current_sub_string);
    encrypt(current_sub_string,our_key);
    //printf("\nTHREAD[%d] substring after encryption= %s",current_thread.m_id,current_sub_string);
    /// now append this to the global result string
//    pthread_mutex_lock(&our_mutex);
//    strcat(result_string, current_sub_string);
//    pthread_mutex_unlock(&our_mutex);
    ((my_thread*)thread)->m_string[(MAX_SIZE/LIST_SIZE)]='\0';
    strcat(current_thread_ptr->m_string, current_sub_string); // copies each encrypted substring to the thread field
    /// works but somehow the m_string is empty outside the function
    //printf("thread[%d]->m_sring=%s\n",current_thread.m_id,((my_thread*)thread)->m_string);




}



void* thread_decrypt_function(void* thread)
{
//    my_thread current_thread= *((my_thread*) thread);
//    print_mythread_info(current_thread);
    my_thread* current_thread_ptr=((my_thread*)thread);
    my_thread current_thread=*((my_thread*)thread);
    size_t length= current_thread.m_end_index-current_thread.m_start_index+1;
    char current_sub_string[length];
    current_sub_string[length-1]='\0';
    /// now lets get to real work and call the desired function
    strncpy(current_sub_string, our_string + current_thread.m_start_index, length);
    current_sub_string[length] = '\0';  // Add null terminator
    printf("\nTHREAD[%d] substring before encryption= %s",current_thread.m_id,current_sub_string);
    decrypt(current_sub_string,our_key);

    ((my_thread*)thread)->m_string[(MAX_SIZE/LIST_SIZE)]='\0';
    strcat(current_thread_ptr->m_string, current_sub_string);


}




void start_multithreading(char indicator, int key, char* data, int char_count)
{
    /// lets divide tasks for each thread based on how many chars we have
    char_count= strlen(data);
    printf("\nchar counter is : %d\n",char_count);
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


                if(pthread_create(&(thread_list[i].m_thread), NULL, thread_encrypt_function, &thread_list[i])!=0)
                    printf("ERROR CREATING THREAD[%d]",i);

            }

        }
    }
    else /// indicator='d'
    {
        for (int i = 0; i < LIST_SIZE; ++i) {

            my_thread current_thread = thread_list[i];

            if (current_thread.m_is_active == 1)/// means its active

            {

                if(pthread_create(&(thread_list[i].m_thread), NULL, thread_decrypt_function, &thread_list[i])!=0)
                    printf("ERROR CREATING THREAD[%d]",i);

            }

        }
    }
}



























/////////////////////////////////////
int main(int argc, char *argv[])

{
    ////// section for complexity in ms
    clock_t start_time, end_time;
    double execution_time;
    start_time=clock();
    /////////////////////////
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
	if (counter > 0) {

        char lastData[counter];
        lastData[0] = '\0';
        strncat(lastData, data, counter);
        lastData[counter - 1] = '\0';
        //printf("lastdata[counter]=%c",lastData[counter]);
        char *action = argv[2];
        strcpy(our_string, lastData);
        our_key = key;
        if (action[1] == 'e') {
            printf("\nour string is :\n%s\n", lastData);
            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('e', key, lastData, counter);
            encrypt(lastData, key);
            printf("Encrypted data:\n%s\n", lastData);
//            decrypt(lastData,key);
//            printf("Decrypted data:\n%s\n",lastData);
        } else {// TODO "-d" as for Decrypt
            printf("\nour string is :\n%s\n", lastData);
            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('d', key, lastData, counter);
            decrypt(lastData, key);
            printf("Decrypted data:\n%s\n", lastData);
        }
        data[counter] = '\0'; // TODO to make sure the string is printed without extra garbage letters
//        printf("original data:\n %s\n",data);
//		printf("Encrypted data:\n %s\n",lastData);

//        printf("\nTHE GLOBAL RESULT STRING IS %s\n",result_string);
//        if(strcmp(result_string,lastData)==0)
//            printf("STRINGS EQUAL TO EXPECTED\n");
//            else
//        {
//            printf("STRINGS NOT EQUAL\n");
//        }

        /*
         * join threads
         */
        for (int i = 0; i < LIST_SIZE; ++i) {
            if (thread_list[i].m_is_active == 1)
                if (pthread_join(thread_list[i].m_thread, NULL) != 0)
                    printf("ERROR JOINING THREAD [%d]", i);
        }
        for (int i = 0; i < LIST_SIZE; ++i) {
            strcat(result_string,thread_list[i].m_string);
            printf(" string of thread[%d] =%s",i,thread_list[i].m_string);

        }
        printf("\nTHE GLOBAL RESULT STRING IS %s\n", result_string);
        printf("\nTHE lastdata STRING IS %s\n", lastData);
        if (strcmp(result_string, lastData) == 0)
            printf("STRINGS EQUAL TO EXPECTED\n");
        else {
            printf("STRINGS NOT EQUAL\n");

        }

    }
    ////// section for complexity print in ms
    end_time=clock();

    execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.6f seconds\n", execution_time);

////////////////////////////

	return 0;
}
/*
 * TODO i need to make sure each and every char[] is null terminated in my program.
 * TODO 1) char string[length];
 * TODO 2) string[length-1]='\0';
 */