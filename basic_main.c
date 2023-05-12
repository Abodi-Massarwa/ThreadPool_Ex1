#include "codec.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t foo_mutex;
/////////////////////////////////////////////////
/*
 * TODO create a structure to hold threads dynamically and assigns tasks to them
 * TODO since we have no clue what the file size would be we cant create as many threads as we want
 */
#include <pthread.h>
#include <stdio.h>
#include <omp.h>

//typedef struct my_thread {
//    int m_id;
//    pthread_t m_thread;
//    struct my_thread* m_next;
//    char* m_task_line;
//
//
//
//}my_thread;
//
//typedef struct {
//    my_thread* m_head;
//
//}my_thread_pool;
//////////////////////////////////////////////
void* foo(void* id)
{
    pthread_mutex_lock(&foo_mutex);
    int x= *((int*) id);
    printf("\nThread Id : %d",x);
    pthread_mutex_unlock(&foo_mutex);
}
pthread_mutex_t foo_mutex;
int main(int argc, char *argv[])
{
    clock_t start_time, end_time;
    double execution_time;
    start_time=clock();
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

//  pthread_t list[1000];
//    for (int i = 0; i < 1000; ++i) {
//        //pthread_mutex_lock(&foo_mutex);
//        pthread_create(&list[i],NULL,foo,(void*)&i);
//        //pthread_mutex_unlock(&foo_mutex);
//    }
  end_time=clock();

   execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.6f seconds\n", execution_time);

	return 0;
}
