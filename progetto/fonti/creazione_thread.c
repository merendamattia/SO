#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define DIM 1000

int something_worked(void) {
    /* thread operation might fail, so here's a silly example */
    void *p = malloc(100);
    free(p);
    return p ? 1 : 0;
}

void *myThread(int *arg) {
  if (something_worked()) {
    int sum = 0;

    for(int i = 1; i < DIM + 1; i++) {
      sum += (i * (int)arg);
      //printf ("Thread %d: i%d, sum: %d\n", pthread_self(), i, sum);
      //sleep (1);
    }

    pthread_exit(sum);

   } else 
       pthread_exit(0);
}

int calculateByProcess(int value_td1, int value_td2, int value_td3, int value_td4, int value_td5, int value_td6){
  int sum = 0;
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td1);
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td2);
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td3);
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td4);
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td5);
  for(int i = 1; i < DIM + 1; i++)
    sum += (i * value_td6);

  return sum;
}

int main() {
  clock_t t;
  t = clock();
  
  pthread_t tid1;
  pthread_t tid2;
  pthread_t tid3;
  pthread_t tid4;
  pthread_t tid5;
  pthread_t tid6;

  void *status1 = 0;
  void *status2 = 0;
  void *status3 = 0;
  void *status4 = 0;
  void *status5 = 0;
  void *status6 = 0;

  int value_td1 = 2;
  int value_td2 = 5;
  int value_td3 = 7;
  int value_td4 = 11;
  int value_td5 = 13;
  int value_td6 = 17;

  int result;

  // -------------------------- Create thread --------------------------
  pthread_create(&tid1, NULL, myThread, value_td1);
  pthread_create(&tid2, NULL, myThread, value_td2);
  pthread_create(&tid3, NULL, myThread, value_td3);
  pthread_create(&tid4, NULL, myThread, value_td4);
  pthread_create(&tid5, NULL, myThread, value_td5);
  pthread_create(&tid6, NULL, myThread, value_td6);

  // -------------------------- Join --------------------------
  pthread_join(tid1, &status1);
  pthread_join(tid2, &status2);
  pthread_join(tid3, &status3);
  pthread_join(tid4, &status4);
  pthread_join(tid5, &status5);
  pthread_join(tid6, &status6);

  // -------------------------- CHECK Status Thread --------------------------
  if(status1 != 0 && status2 != 0 && status3 != 0 && status4 != 0 && status5 != 0 && status6 != 0){
    result = (int)status1 + (int)status2 + (int)status3 + (int)status4 + (int)status5 + (int)status6;
    printf("Result by threads: %d\n", result);
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
  printf("Threads took %f seconds to execute \n", time_taken);

  t = clock();

  printf("Result by process: %d\n", calculateByProcess(value_td1, value_td2, value_td3, value_td4, value_td5, value_td6));

  t = clock() - t;
  time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
  printf("Process took %f seconds to execute \n", time_taken);


  return 0;
}
