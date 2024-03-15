#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define NUM_BUCKETS 5
#define NUM_THREADS 5
#define ARRAY_SIZE 50


int array[ARRAY_SIZE];
int buckets[NUM_BUCKETS][ARRAY_SIZE] = {0};
int bucket_sizes[NUM_BUCKETS] = {0};



pthread_mutex_t mutexes[NUM_BUCKETS];
void* sort_bucket(void* arg) {
    int index = *(int*)arg;
    pthread_mutex_lock(&mutexes[index]);
    for (int i = 1; i< bucket_sizes[index];i++){
        int key = buckets[index][i];
        int j = i - 1;
        while (j >= 0 && buckets[index][j]> key){
            buckets[index][j + 1]= buckets[index][j];
            j= j - 1;
        }
        buckets[index][j + 1] = key;
  }
    pthread_mutex_unlock(&mutexes[index]);
    pthread_exit(NULL);
}
void bucket_sort_pthreads() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
        thread_args[i] = i;
        pthread_create(&threads[i],NULL, sort_bucket,(void*)&thread_args[i]);
    }
    for (int i = 0; i < NUM_THREADS;i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX;
    }
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int index = array[i] / (MAX / NUM_BUCKETS);
        pthread_mutex_lock(&mutexes[index]);
        buckets[index][bucket_sizes[index]++] = array[i];
        pthread_mutex_unlock(&mutexes[index]);
    }
    
    bucket_sort_pthreads();
    int index = 0;
    for(int i= 0; i< NUM_BUCKETS;i++){
        for (int j = 0; j< bucket_sizes[i];j++){
            array[index++]= buckets[i][j];
       }
     }
    for (int i = 0; i < ARRAY_SIZE; i++){
        printf("%d ",array[i]);
     }
     printf("\n");
}