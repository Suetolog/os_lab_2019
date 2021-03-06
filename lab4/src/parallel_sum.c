#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <getopt.h>
#include "utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  struct SumArgs *s_args = (struct SumArgs *)args;
  // TODO: your code here 
  for(int i = s_args -> begin; i < s_args->end; ++i)
	{
		sum += (s_args->array)[i];
	}
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  //pthread_t threads[threads_num];

  /*
   * TODO:
   * your code here
   * Generate array here
   */
	
	while (true) {
    int current_optind = optind ? optind : 1;
    
    static struct option options[] = {{"seed", required_argument, 0,},
                                      {"threads_num", required_argument, 0,},
                                      {"array_size", required_argument, 0,},
                                      {0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1) break;
		switch (c) {
		  case 0:
			switch (option_index) {
			  	case 0:
				{
					seed = atoi(optarg);
					if (seed <= 0) {
						printf("seed is a positive number\n");
						return 1;
					}
					break;
				}
				case 1:
				{
					threads_num = atoi(optarg);
					if (threads_num <= 0) {
						printf("Threads num is a positive number\n");
						return 1;
					}
					break;
				}
				case 2:
				{
					array_size = atoi(optarg);
					if(array_size <= 0)
					{
						printf("Array size is a positive number\n");
						return 1;
					}

					break;
				}
				defalut:
					printf("Index %d is out of options\n", option_index);
			}
			break;

			case '?':
				break;

			default:

			printf("getopt returned character code 0%o?\n", c);
		}
	}

	if (optind < argc) {
		printf("Has at least one no option argument\n");
		return 1;
	}
	
	if (seed == -1 || array_size == -1 || threads_num == -1 ) {
		printf("Usage: %s --seed \"num\" --array_size  \"num\" --threads_num \"num\" \n",
				argv[0]);
		return 1;
	}

	pthread_t *threads = malloc( sizeof(pthread_t) * threads_num );

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

   printf("\nArray: ");
		
		for(int i = 0; i < array_size; ++i)
		{
			printf("%d ", array[i]);
		}
		printf("\n");

  //struct SumArgs args[threads_num];
  struct SumArgs* args = malloc(sizeof(struct SumArgs) * threads_num);
	
  int block = array_size / threads_num;
	for(int i = 0; i < threads_num; ++i)
	{
		args[i].array = array;
		args[i].begin = block*i;
		
		if(i == threads_num - 1)
		{
			args[i].end = block * (i+1) + array_size % threads_num;
		}
		else
		{
			args[i].end = block * (i+1);
		}
	}
	struct timeval start_time;
	gettimeofday(&start_time, NULL);

  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }
  printf("\n");
  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    printf("Sum_%d: %d\n", i+1, sum);
    total_sum += sum;
  }
  struct timeval finish_time;
	gettimeofday(&finish_time, NULL);
	double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
	elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);
    free(args);
	free(threads);
    printf("Total: %d\n", total_sum);
	printf("\nElapsed time: %fms\n\n", elapsed_time);
  
  return 0;
}
