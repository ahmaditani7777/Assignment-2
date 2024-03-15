#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define NUM_BUCKETS 5
#define ARRAY_SIZE 50


int array[ARRAY_SIZE];
int buckets[NUM_BUCKETS][ARRAY_SIZE];
int bucket_sizes[NUM_BUCKETS] = {0};

void sort_bucket(int* bucket, int size){
    for (int i = 1;i < size;i++) {
        int key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j]>key){
             bucket[j+1] = bucket[j];
            j = j - 1;
          }
        bucket[j+1] = key;
    }
}
void bucket_sort_openmp() {
    #pragma omp parallel for
    for (int i= 0 ;i< ARRAY_SIZE;i++) {
        int index = array[i] / (MAX / NUM_BUCKETS);
        #pragma omp critical
        {
          buckets[index][bucket_sizes[index]++] = array[i];
        }
  }
    #pragma omp parallel for
    for (int i = 0 ;i< NUM_BUCKETS;i++) {
        sort_bucket(buckets[i], bucket_sizes[i]);
    }}
int main() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX;}
    bucket_sort_openmp();
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < bucket_sizes[i]; j++) {
            array[index++] = buckets[i][j];
        }
    } 
    for (int i= 0;i< ARRAY_SIZE;i++){
        printf("%d ", array[i]);
     }
    printf("\n");
    return 0;
}