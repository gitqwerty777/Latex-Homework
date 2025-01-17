/* begin */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXN 20
int n; /* a global n */
/* ok */
int ok(int position[], int next, int test)
{
  for (int i = 0; i < next; i++)
    if (position[i] == test || 
	(abs(test - position[i]) == next - i))
      return 0;
  return 1;
}
/* queen */
int queen(int position[], int next)
{
  if (next >= n)
    return 1;
  int sum = 0;
  for (int test = 0; test < n; test++) 
    if (ok(position, next, test)) {
      position[next] = test;
      sum += queen(position, next + 1);
    }
  return sum;
}
/* go */
void *goQueen(void *pos)
{
  int *position = (int *)pos;
  int *num = (int *)malloc(sizeof(int));
  *num = queen(position, 1);
#ifdef VERBOSE
  printf("goQueen: thread %d, # of solution = %d\n", 
	 position[0], *num);
#endif
  pthread_exit(num);
}
/* main */
int main (int argc, char *argv[])
{
  assert(argc == 2);
  n = atoi(argv[1]);
  assert(n <= MAXN);
  int *position;
  pthread_t threads[MAXN];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, 
    PTHREAD_CREATE_JOINABLE);
  /* declaration_end */
  for (int i = 0; i < n; i++) {
    position = (int *)calloc(n, sizeof(int));
    assert(position != NULL);
    position[0] = i;
    int error = pthread_create(&threads[i], &attr, goQueen, 
			       (void *)position);
    assert(error == 0);
  }
  pthread_attr_destroy(&attr);
  /* join */
  int numSolution = 0;
  for (int i = 0; i < n; i++) {
    int *num;
    pthread_join(threads[i], (void **)&num);
    numSolution += *num;
#ifdef VERBOSE
  printf("main: thread %d done\n", i);
#endif
  }
  printf("total # of solution %d\n", numSolution);
  pthread_exit(NULL);
  return 0;
}
/* end */
