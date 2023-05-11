#include "codec.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/////////////////////////////////////////////////
/*
 * TODO create a structure to hold threads dynamically and assigns tasks to them
 * TODO since we have no clue what the file size would be we cant create as many threads as we want
 */
typedef struct my_thread {
    int m_id;
    pthread_t m_thread;
    struct my_thread* m_next;
    char* m_task_line;



}my_thread;

typedef struct {
    my_thread* m_head;

}my_thread_pool;
//////////////////////////////////////////////

int main(int argc, char *argv[])
{
//	char data[] = "Hello world";
//	int key = 12;
//
//     TODO:MY THOUGHTS
//     * it appears both encrypt and decrypt functions modify the original data passed to them
//     * as an array of chars which is equivalent to char*
//     * flashback from C : you better not use the original pointer in the process of iterating since you
//     * would change its address
//
//	encrypt(data,key);
//	printf("encrypted data: %s\n",data);
//
//	decrypt(data,key);
//	printf("decrypted data: %s\n",data);

  /*  my_thread* t1=(my_thread*)calloc(1,sizeof(my_thread));

    t1->m_id=69;
    t1->m_next=(my_thread*) calloc(1, sizeof(my_thread));
    pthread_t p1;
    t1->m_thread=p1;

    printf("id of thread 1 :%d",t1->m_id);
    printf("id of thread 1 %d",t1->m_thread);*/


	return 0;
}
