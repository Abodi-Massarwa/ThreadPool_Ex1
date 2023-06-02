#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
///////////////////////////////////
///// INITS //////
#define LIST_SIZE 6
#define MAX_SIZE 1024
///////////////////////// Dynamic array //////////////////////////////////////
struct dynamic_string
{
    char* m_string;
    int m_alloc_size; /// not till \0 but the size of alloc
    int m_index;
};
typedef struct dynamic_string dynamic_string;
/*
 * initializing the string initially with 1024
 */
void init(dynamic_string* str)
{
    str->m_alloc_size=MAX_SIZE+100;
    str->m_index=0;
    str->m_string=(char*) calloc(str->m_alloc_size, sizeof(char));
    /// TODO assuming calloc nullpoints the whole array
}
/*
 * appending I went with chars sine its the most used thing in C we aren't regular to pushing strings
 *
 */
void append(dynamic_string* str, char c)
{
    if(str->m_index==str->m_alloc_size) // we need to resize
    {
        /// resizing the array
        str->m_alloc_size+=MAX_SIZE; // increasing with multiples of 1024
        str->m_string=(char*) realloc(str->m_string, (str->m_alloc_size) * sizeof(char));
    }
    str->m_string[str->m_index++]=c; // added the char and index++
    str->m_string[str->m_index]=0; /// null point after each char since we dont know when to stop.
}
void append_string(dynamic_string* str, char* c)
{
    if(str->m_index==str->m_alloc_size || strlen(c) >= (str->m_alloc_size - str->m_index + 1)) // we need to resize
    {
        /// resizing the array
        str->m_alloc_size+=MAX_SIZE; // increasing with multiples of 1024
        str->m_string=(char*) realloc(str->m_string, (str->m_alloc_size) * sizeof(char));
    }
    for (int i = 0; i < strlen(c); ++i) {
        str->m_string[str->m_index++]=c[i];
    }
    //str->m_string[str->m_index++]=c; // added the char and index++
    str->m_string[str->m_index]=0; /// null point after each char since we dont know when to stop.
}
//int check(dynamic_string* str)
//{
//    while(str->m_index!='\0')
//        ++str->m_index;
//    if(str->m_index== strlen(str->m_string)-1)
//    {
//        str->m_alloc_size+=MAX_SIZE; // increasing with multiples of 1024
//        str->m_string=(char*) realloc(str->m_string,str->m_alloc_size* sizeof(char));
//    }


void release(dynamic_string* str)
{
    free(str->m_string);
}
struct my_thread {
    int m_id; // TODO assuming it helps us in the matter or putting back data in order after the multi-threading is done.
    int m_start_index; /// since we are dealing with char* tasks can be divided by simply indexes
    int m_end_index;
    int m_is_active; /// 1 active 0 not active
    char m_string[(MAX_SIZE / LIST_SIZE) + 1]; /// worst case scenario each thread holds 1024/6 chars
    pthread_t m_thread;
    struct dynamic_string str;
};
typedef struct my_thread my_thread;
//////////////////// GLOBAL VARIABLES SECTION ////////////////////////////////
my_thread thread_list[LIST_SIZE]; // our array of threads with a fixed size of 6
char our_string[MAX_SIZE] = {0};
char result_string[MAX_SIZE] = {0};
int our_key;
dynamic_string final_str;
dynamic_string global_str;
////////////////////////////// functions //////////////////////////////////
void print_mythread_info(my_thread thread) {
    //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\nthread_list[%d]", thread.m_id);
    printf("\nstart index=%d , end index=%d", thread.m_start_index, thread.m_end_index);
    printf("\nis active ?");
    if (thread.m_is_active == 1)
        printf(" True");
    else /// thats extra we dont even receive dead threads in this function.
        printf(" False");
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("\nstring length is %ld",strlen(thread.str.m_string));
    printf("\nstring is %s",thread.str.m_string);
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

void *thread_encrypt_function(void *thread) {
    /*
     *  i just realized we need to give each thread its own char[] to prevent shared variables and race conditions
     *  and to not use mutex which is gonna be costy
     *  TODO 1) lets give each thread its own char[]
     */


//    my_thread *current_thread_ptr = ((my_thread *) thread);
//    my_thread current_thread = *((my_thread *) thread);
//    size_t length = current_thread_ptr->m_end_index - current_thread_ptr->m_start_index + 1;
//    char current_sub_string[length + 1];
//    current_sub_string[0] = '\0';
//    /// now lets get to real work and call the desired function
//    strncpy(current_sub_string, our_string + current_thread.m_start_index, length);
//    current_sub_string[length] = '\0';  // Add null terminator
//    //////////////////////////////////////////////////////////
//    encrypt(current_sub_string, our_key);
//
//    strcpy(current_thread_ptr->m_string, current_sub_string);
//    current_thread_ptr->m_string[length] = '\0';

/* TODO
 *  in this step we are getting a thread in which it has a start index and an end index which could be <1024 or even more
 *  so we need to do our math right and check for both cases
 */
    my_thread * current_thread_ptr= (my_thread*)thread;
    size_t start_index=current_thread_ptr->m_start_index;
    size_t end_index=current_thread_ptr->m_end_index;
    int size=end_index-start_index+1;
    int iterator=start_index;
    int remainder= size%1024; //could be 0 could be k<1024
    int k=floor(size/1024); // could be 0 could be time of 1024
    //// TODO delete before submit !!!
    //printf("\nthread [%d] string size is %ld",current_thread_ptr->m_id,strlen(current_thread_ptr->str.m_string));
//    printf("\nportion size mod 1024 is %d",remainder);
//    printf("\nportion number is%d",k);
    /// initializing the struct dynamic_string inside our thread
    dynamic_string dynamic_thread_str =current_thread_ptr->str;
    init(&dynamic_thread_str);
    dynamic_thread_str.m_string=(char*) calloc(size + 1, sizeof(char)); /// allocating string same size as for strcat in future
    ////////////////////////////////////////////////////////// TODO FIX THE ERROR IN HERE THE DIVISION IS MISSED UP
    char* temp_string=(char*) calloc(1024+1, sizeof(char)); ///TODO FREEEEE 1024+1 for the null pointer.
    for (int i = 0; i <k; ++i)
    {
        /// time to do encrypt/decrypt for each chunk at a time
        /*
         * TODO for each chunck out of k we need to :
         *      1) copy it to a temporary string from the string we got as input
         *      2) call encrypt/decrypt with that @temp_string (strcpy)
         *      3) append it to our @thread->str->m_string (srtncat)
         *
         */
        strncpy(temp_string, global_str.m_string + iterator, 1024);
        iterator+=1024;
        temp_string[1024]='\0';
        encrypt(temp_string,our_key);
        append_string(&current_thread_ptr->str,temp_string);
        //strncat(current_thread_ptr->str.m_string,temp_string, strlen(temp_string)+1); // +1 for \0

    }
    /// after all of this there might be a remainder soo .
    if(remainder>0) {
        strncpy(temp_string, global_str.m_string + iterator, remainder);
        temp_string[remainder] = '\0';
        //// TODO delete
        //printf("\nstrlen of %d", strlen(temp_string));
        encrypt(temp_string, our_key);
        //check(&current_thread_ptr->str);
        append_string(&current_thread_ptr->str,temp_string); /// TODO check its functionality in worse cases
       //printf("strlen %d\n", strlen(current_thread_ptr->str.m_string));
       //printf("index %d", current_thread_ptr->str.m_index);
        /// TODO in case of cat we need to be cautios and resize our string
        //strncat(current_thread_ptr->str.m_string, temp_string, remainder + 1);
//        int portion_size= remainder-iterator+1;
//        iterator+=remainder;
        dynamic_thread_str.m_string[current_thread_ptr->str.m_index]='\0'; /// TODO check index
        //printf("\nstrlen of thread string %d", strlen(current_thread_ptr->str.m_string));
    }
//// TODO check ur math in the whole algorithm

}


void *thread_decrypt_function(void *thread) {

//    my_thread *current_thread_ptr = ((my_thread *) thread);
//    my_thread current_thread = *((my_thread *) thread);
//    size_t length = current_thread_ptr->m_end_index - current_thread_ptr->m_start_index + 1;
//    char current_sub_string[length + 1];
//    current_sub_string[0] = '\0';
//    /// now lets get to real work and call the desired function
//    strncpy(current_sub_string, our_string + current_thread.m_start_index, length);
//    current_sub_string[length] = '\0';  // Add null terminator
//    //////////////////////////////////////////////////////////
//    decrypt(current_sub_string, our_key);
//
//    strcpy(current_thread_ptr->m_string, current_sub_string);
//    current_thread_ptr->m_string[length] = '\0';

    /*
     *  NEW SOLUTION
     *
     */

    /* TODO
 *  in this step we are getting a thread in which it has a start index and an end index which could be <1024 or even more
 *  so we need to do our math right and check for both cases
 */
    my_thread * current_thread_ptr= (my_thread*)thread;
    size_t start_index=current_thread_ptr->m_start_index;
    size_t end_index=current_thread_ptr->m_end_index;
    int size=end_index-start_index+1;
    int iterator=start_index;
    int remainder= size%1024; //could be 0 could be k<1024
    int k=floor(size/1024); // could be 0 could be time of 1024
    /// initializing the struct dynamic_string inside our thread
    dynamic_string dynamic_thread_str =current_thread_ptr->str;
    init(&dynamic_thread_str);
    dynamic_thread_str.m_string=(char*) calloc(size + 1, sizeof(char)); /// allocating string same size as for strcat in future
    //////////////////////////////////////////////////////////
    char* temp_string=(char*) calloc(1024+1, sizeof(char)); ///TODO FREEEEE 1024+1 for the null pointer.
    for (int i = 0; i < k; ++i)
    {
        /// time to do encrypt/decrypt for each chunk at a time
        /*
         * TODO for each chunck out of k we need to :
         *      1) copy it to a temporary string from the string we got as input
         *      2) call encrypt/decrypt with that @temp_string (strcpy)
         *      3) append it to our @thread->str->m_string (srtncat)
         *
         */
        strncpy(temp_string, global_str.m_string + iterator, 1024);
        iterator+=1024;
        temp_string[1024]='\0';
        decrypt(temp_string,our_key);
        append_string(&current_thread_ptr->str,temp_string);
        //strncat(current_thread_ptr->str.m_string,temp_string, strlen(temp_string)+1); // +1 for \0

    }
    /// after all of this there might be a remainder soo .
    if(remainder>0) {
        strncpy(temp_string, global_str.m_string + iterator, remainder);
        temp_string[remainder] = '\0';
        decrypt(temp_string, our_key);
        //check(&current_thread_ptr->str);
        append_string(&current_thread_ptr->str,temp_string); /// TODO check its functionality in worse cases
        //printf("strlen %d\n", strlen(current_thread_ptr->str.m_string));
        //printf("index %d", current_thread_ptr->str.m_index);
        /// TODO in case of cat we need to be cautios and resize our string
        //strncat(current_thread_ptr->str.m_string, temp_string, remainder + 1);
        //dynamic_thread_str.m_string[end_index] = '\0';
    }

}
void start_multithreading(char indicator, int key, char *str, int index)
{
    /*
     * giving indexes for each thread;
     */
    int length= strlen(str); // till \0
    int total=length;
    int iterator=0;
    int remainder=0;
    int chunk_size=-1;
    int remaining=-1;
    if(length%LIST_SIZE!=0) ///not  multiple of 6
    {
        /*
         * 1) might be less than 1024
         * 2) might be 1024*k + remainder
         */
        remainder=length%LIST_SIZE;
    }
    total-=remainder;
    chunk_size=total/LIST_SIZE;
    remaining=length;// includes the remainder in case it exists.
    /// the portion for each thread to take.
    /// regarding the remainder we could give it to last thread or first doesn't really matter.
    //// once again chunk size could be zero , maybe we should pass only strings with at least 1 char in previous block

    for (int i = 0; i < LIST_SIZE; ++i)
    {
        //TODO give indexes for each thread.
        if(remaining!=remainder) {
            thread_list[i].m_start_index = iterator;
            thread_list[i].m_end_index = thread_list[i].m_start_index + chunk_size - 1;
            iterator += chunk_size;
            remaining -= chunk_size;
            thread_list[i].m_is_active=1;
        }
    }
    /// we still have remainder to deal with we add it to the last thread.
    thread_list[5].m_end_index+=remainder;
//    /// DONE !
//    for (int i = 0; i < LIST_SIZE; ++i) {
//        print_mythread_info(thread_list[i]);
//    }



//void start_multithreading(char indicator, int key, char *data, int index) {
//    /// lets divide tasks for each thread based on how many chars we have
//    int char_count = 0;
//    char_count = strlen(data); /// length
//    int remainder = 0; /// remainder
//    int chunk_size = char_count / LIST_SIZE;
//    int current_char_count = char_count;
//    int index_counter = 0;
//    if (char_count % LIST_SIZE != 0)
//        remainder = char_count % LIST_SIZE;
//    /*
//     * TODO SUSSYYYYYYY
//     */
//    for (int i = 0; i < LIST_SIZE; ++i) {
//        if (current_char_count != 0) {
//            thread_list[i].m_is_active = 1; /// set to true
//            thread_list[i].m_start_index = index_counter;
//            index_counter = index_counter + chunk_size;
//            thread_list[i].m_end_index = index_counter - 1; /// we'll -1 each one in a second loop
//            current_char_count -= chunk_size;
//        }
//        if (i == 0) {
//            thread_list[i].m_end_index += remainder;
//            index_counter += remainder;
//            current_char_count -= remainder;
//        }
//    }
//
///*
// * TODO based on the char ('e' | 'd') we know whether its Encrypt or Decrypt to call
// * /// so now our thread_list is ready
    if (indicator == 'e') {
        for (int i = 0; i < LIST_SIZE; ++i) {

            my_thread current_thread = thread_list[i];

            if (current_thread.m_is_active == 1)/// means its active

            {


                if (pthread_create(&(thread_list[i].m_thread), NULL, thread_encrypt_function, &thread_list[i]) != 0)
                    printf("ERROR CREATING THREAD[%d]", i);

            }

        }
    } else /// indicator='d'
    {
        for (int i = 0; i < LIST_SIZE; ++i) {

            my_thread current_thread = thread_list[i];

            if (current_thread.m_is_active == 1)/// means its active

            {

                if (pthread_create(&(thread_list[i].m_thread), NULL, thread_decrypt_function, &thread_list[i]) != 0)
                    printf("ERROR CREATING THREAD[%d]", i);

            }

        }
    }
}


////////////////MAIN//////////////////
/////////////////////////////////////
int main(int argc, char *argv[])
/* TODO 1) make sure each empty char[] is null-terminated (use memset() or init char example[length]={0})
 *      2) make sure strcpy,strncpy | strcar,strncat are called correctly .
        3) there is nothing to terminate in my code , so job's easy :)
 */

{
    init(&final_str);
    init(&global_str);
    ////// section for complexity in ms
    clock_t start_time, end_time;
    double execution_time;
    start_time = clock();
    /////////////////////////
    int dev_mode = 0;
    ////////////////////////
    for (int i = 0; i < 6; ++i) {
        /*
         *TODO assigning id to each thread to override the auto-init trash values
         * TODO regarding threads themselves tbh no one cares what values they hold . what matters the most
         * TODO is to get them to work
         */
        thread_list[i].m_id = i;
    }
    /*
     * get the threads to work using the relevant functions
     * TODO 1) pthread_t_create
     * TODO 2) pthread_t_join
     * TODO etc....
     */
    if (argc < 3) {
        printf("usage: key -[e|d] < file OR stream \n");
        printf("!! data more than 1024 char will be ignored !!\n");
        return 0;
    }
    if (argc == 4)/// for testing purposes
        dev_mode = 1;

    int key = atoi(argv[1]);
    //printf("key is %i \n",key);

    int c;
    int input_indicator;
    int counter = 0;
    int dest_size = 1024;
    char data[dest_size + 1];
    // Null-terminate the entire string
    for (size_t i = 0; i <= dest_size; i++) {
        data[i] = '\0';
    }


    if (isatty(fileno(stdin))) {
        // Input is not redirected, read from command line

        //printf("Enter input from command line:\n");

        input_indicator = '\n';
    } else {
        input_indicator = EOF;
    }
//    while (((c = getchar()) != input_indicator)  &&(counter != 1024))
//    { /// Either EOF| \n or index reached 1024(which should be \0)
//        /// in case it didnt reach 1024 then it reached index k which we should null terminate. (to indicate the end of string)
//        data[counter] = c;
//        counter++;
//    }
    //////////// INIT //////////////////
    //dynamic_string str;

    //TODO alternate version of while for our new dynamic string.
    while (((c = getchar()) != input_indicator))
    { /// stops only in case we encounter an indicator.
        counter++;
        //data[counter] = c;
        append(&global_str,(char) c);
        /*
         *      TODO it works !!!, once again remember
         *          1) m_alloc_size means allocated size
         *          2) use strlen() to tell whats the real size till last letter.
         *          3) strlen() doesnt include the \0 in its count.
         */
        /*
         *  TODO we need to modify threads
         *      1) to have dynamic_string
         *      2) or give start\ end index for each thread in the global string array
         *
         */
    }


    /*
     * after we exit the loop we need to make sure whether the text reached the maximum length
     * or simply because the file ended and the counter isn't 1024
     */
    if (counter > 0) {

        char lastData[counter + 1]; /// same size as the input.
        lastData[0] = '\0';
        data[counter] = '\0';
        strcat(lastData, data);
        lastData[counter] = '\0'; /// double-checked no need for more headaches

        char *action = argv[2];
        strcpy(our_string, lastData);
        our_key = key;
        ////strcmp extra section
//        if(
//        strcmp("HkcfcUXgX]jYf[YX]bUmY``ckkccX ",lastData)==0)
//            printf("TRUE\n");
//        else
//            printf("FALSE THERE IS SOMETHING WRONG\n");
        if (action[1] == 'e') {

            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('e', key, global_str.m_string, counter);
            if (dev_mode) {
                printf("\nour original string is :\n%s\n", lastData);
                encrypt(lastData, key);
                printf("Encrypted single-threaded data:\n%s\n", lastData);
            }

        } else {// TODO "-d" as for Decrypt
            // TODO here we need to call pthread_t_create but before we need to divide the job on threads
            start_multithreading('d', key, global_str.m_string, counter);
            if (dev_mode) {
                printf("\nour original string is :\n%s\n", lastData);
                decrypt(lastData, key);
                printf("Decrypted single-threaded data:\n%s\n", lastData);
            }
        }
        /*
         * join threads (last station)
         */
        for (int i = 0; i < LIST_SIZE; ++i) {
            if (thread_list[i].m_is_active == 1)
                if (pthread_join(thread_list[i].m_thread, NULL) != 0)
                    printf("ERROR JOINING THREAD [%d]", i);
        }
        /*
         * start appending each thread m_string to the global final result string to form our whole Encrypted/Decrypted string
         * (I could've done it without global since no such functions accessed it )
         */
        for (int i = 0; i < LIST_SIZE; ++i) {
            //TODO make sure each thread string is null terminated and doesnt contain garbage.
            //  we dont want to see garbage characters which we aren't interested in :'(
            //strcat(result_string, thread_list[i].m_string);
            append_string(&final_str,thread_list[i].str.m_string);
            //print_mythread_info(thread_list[i]);
            //printf(" string of thread[%d] =%s", i, thread_list[i].m_string);

        }
        /// TODO DELETE
//        printf("\nsize of first string :%ld",strlen(global_str.m_string));
//        printf("\n%s",global_str.m_string);
//
//        printf("\nsize of final string :%ld",strlen(final_str.m_string));
//        printf("\n%s",final_str.m_string);
        /*
         * Printing the final result
         */
        if (dev_mode) {
            if (action[1] == 'e')
                printf("\nthe Encrypted multi-threaded string is:\n%s\n", result_string);
            else //// means 'd'
                printf("\nthe Decrypted multi-threaded string is:\n%s\n", result_string);


            if (strcmp(result_string, lastData) == 0)
                printf("STRINGS EQUAL TO EXPECTED\n");
            else {
                printf("STRINGS NOT EQUAL\n");

            }




            ////// section for complexity print in ms
            end_time = clock();

            execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
            if (dev_mode)
                printf("\nExecution time: %.6f seconds\n", execution_time);
        }

////////////////////////////
/// we simply print the plain result itself to make it flexible for multiple redirections ... < > <>
        printf("%s", final_str.m_string);
    }
    //// otherwise counter is 0 which means we have no string to Encrypt , Decrypt.
    return 0;
}
/*
 * TODO i need to make sure each and every char[] is null terminated in my program.
<<<<<<< HEAD
 * TODO 1) char string[length];
 * TODO 2) string[length-1]='\0';
=======
>>>>>>> main
 */