#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
int factorial = 1;

struct fact_args {
    int begin;
    int end;
    int mod;
};

void ParFact(void*);

int main(int argc, char **argv)
{
  int k = 0;
  int mod = -1;
  int pnum = -1;
  pthread_t threads[pnum];
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
		printf("Use %s --k \" x\" --pnum \" y \" --mod \" z\" \n",
			argv[0]);
		return 1;
	}
  struct fact_args args[pnum];
  int part=k/pnum;
  for(int i=0;i<pnum;i++){
      args[i].begin=i*part+1;
      args[i].end=(i == (pnum - 1) ) ? k : (i + 1) * part;
      args[i].mod=mod;
  }
  for(int i=0;i<pnum;i++){
      pthread_create(&threads[i], NULL, (void *)ParFact, (void *)&args[i]);
    
  }

  for(int i=0;i<pnum;i++){
    pthread_join(threads[i], NULL);
  }
  printf("Factorial - %d\n",factorial);

}

void ParFact(void* args){
    struct fact_args* str=(struct fact_args*)args;
    int buf=1;
    for(int i=str->begin;i<=str->end;i++){
        buf=((buf*i)%str->mod);
        printf("Buf - %d\n",buf);
    }
    pthread_mutex_lock(&mut);
    factorial=(factorial*buf)%str->mod;
    pthread_mutex_unlock(&mut);
}