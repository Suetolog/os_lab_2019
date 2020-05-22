#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct args_calc
{
    int    start;
    int    end;
    int     mod;
    int*    res;
    
};

int sync_fact(int k, int mod){
    
    for(unsigned int i = k-1; i > 1; i--){
        k = (k * i % mod) % mod;
    }
    return k % mod;    
}

void* thread_factorial(void* args_f)
{
    struct args_calc* af = (struct args_calc*)args_f;
    int start = af -> start;
    int end =   af -> end;
    int mod =   af -> mod;

    int work = start;

    for(int i = start + 1; i <= end; i ++)
    {
        work = (work * (i%mod))%mod;
    }
    pthread_mutex_lock(&mut);
    *(af -> res) = work;
    pthread_mutex_unlock(&mut);
};


int main(int argc, char **argv)
{
    int k = -1;
    int mod = -1;
    int pnum = -1;
    
    while (true)
    {        
        static struct option options[] = {
            {"k", required_argument},
            {"pnum", required_argument},
            {"mod", required_argument},
        };

        int option_index = 0;
        int c = getopt_long(argc, argv,"f", options, &option_index);

        if (c == -1) break;

            switch (c) {
            case 0:
            switch (option_index) {
                case 0:
                {
                k = atoi(optarg);

                if (k <= 0) {
                    printf("k > 0\n");
                    return 1;
                }
                    break;
                }
                case 1:
                {
                pnum = atoi(optarg);

                if (pnum <= 0) {
                    printf("pnum > 0\n");
                    return 1;
                    }
                    break;
                }
                case 2:
                {
                mod = atoll(optarg); 
                if(mod <= 0)
                {
                printf("mod > 0\n");
                    return 1;
                }
                    
                    break;
                }
            }
            break;      
    }
    }
    if (k == -1 || pnum == -1 || mod == -1) {
		printf("Use %s -k \" x\" --pnum \" y \" --mod \" z\" \n",
			argv[0]);
		return 1;
	}

    if (pnum > k/2){
        pnum = pnum / 2;
    }
    
    pthread_t* threads = malloc(pnum*sizeof(pthread_t));    
    struct args_calc* ac = malloc(pnum * sizeof(struct args_calc));
        
    int thread;
    int final_res = 1;
    int block = k / pnum;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
        
    for(int i = 0; i < pnum; i++)
    {
        ac[i].start = block*i + 1;
        ac[i].mod = mod;
        ac[i].res = &final_res;    

        if(i+1 == pnum)
        {
            ac[i].end = block * (i+1) + k % pnum;
        }
        else
        {
            ac[i].end = block * (i+1);
        }    
       
        thread = pthread_create( &threads[i], NULL, thread_factorial, &ac[i]);
    }
    
    for(int i = 0; i < pnum; i++)
    {
       thread = pthread_join( threads[i], NULL);
    }
    
    
    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    

    printf("\nAsync %d! mod %d = %d\n", k, mod, final_res);
    printf("\nSync  %d! mod %d = %d\n", k, mod, sync_fact(k,mod));
    
    free(threads);
    free(ac);

    printf("\nElapsed time: %fms\n\n", elapsed_time);

    return 0;
}