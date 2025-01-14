#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define NUM 40

typedef struct mat mat;
struct mat{
	int **A;
	int **B;
	long long int** C;
	int alloc;
};
void* mul(void *arg);
void add(long long int**);
int main(void){
	mat arg[NUM];
	int i, j;
	FILE *fd1, *fd2;
	time_t start, finish;
	pthread_t tid[NUM];
	int **A = (int**)malloc(sizeof(int*) * 4000);
	int **B = (int**)malloc(sizeof(int*) * 4000);
	long long int **C = (long long int**)malloc(sizeof(long long int*) * 4000);
	for(i = 0; i < NUM; i++) arg[i].A = A, arg[i].B = B, arg[i].C = C, arg[i].alloc = i;
	for(i = 0; i < 4000; i++)
		A[i] = (int*)malloc(sizeof(int)*4000),
		B[i] = (int*)malloc(sizeof(int)*4000), 
		C[i] = (long long int*)malloc(sizeof(long long int) * 4000);
	fd1 = fopen("sample1.txt", "r");
	fd2 = fopen("sample2.txt", "r");
	for(i = 0; i < 4000; i++) for(j = 0; j < 4000; j++) 
		fscanf(fd1, "%d", &(A[i][j])), fscanf(fd2, "%d", &(B[i][j])); 
	printf("Starting multiplication\n");
	start = time(NULL);
	for(i = 0; i < NUM; i++) pthread_create(&tid[i], NULL, mul, (void*)&arg[i]);
	for(i = 0; i < NUM; i++) pthread_join(tid[i], NULL);
	add(C);
	finish = time(NULL);
	printf("Elapsed seconds: %ld", finish - start);
	return 0;
}

void* mul(void *arg){
	int i,j,k;
	mat* argv  = (mat*)arg;
	int **A = argv->A;
	int **B = argv->B;
	long long int **C = argv->C;
	int alloc = argv->alloc;
	long long int sum;
	for(i = 4000/NUM * alloc; i < 4000 / NUM * (alloc + 1); i++){
		for(j = 0; j < 4000; j++){
			sum = 0;
			for(k = 0; k < 4000; k++){
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
			printf("C[%d][%d] done\n",i,j);
		}
	}
}

void add(long long int **C){
	long long int sum = 0;
	int i,j;
	for(i = 0; i < 4000; i++) for(j = 0; j < 4000; j++) sum += C[i][j];
}
