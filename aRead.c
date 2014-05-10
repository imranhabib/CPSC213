#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"


queue_t tempQueue;

typedef struct {
    int blocknumber;
    char* buf;
} pendingRead;

void asyncRead (char* buf, int nbytes, int blockno, void (*handler) (char*, int, int)) {
    return;
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
  free(*tempRead);
}

void interruptServiceRoutine () {
  pendingRead *tempRead = queue_dequeue(&tempQueue);
  printf("BlockNumber: %d \n", (tempRead->blocknumber));
  printf("buffernum: %d \n", (int *) *(tempRead->buf));
  void* (*fp)(char *, int, int) = handleRead;
  asyncRead(tempRead->buf, 4096, tempRead->blocknumber, fp(tempRead->buf, 4096, tempRead->blocknumber));
}

void run (int numBlocks) {
  char buf [8];
  printf("Starting run\n");
  for (int blockno = 0; blockno < numBlocks; blockno++) {
      pendingRead *tempRead = malloc(sizeof(pendingRead));
      tempRead->blocknumber = blockno;
      tempRead->buf = buf;
      queue_enqueue(&tempQueue, tempRead);
      disk_scheduleRead(buf, 4096, blockno);
}
  disk_waitForReads();
}

int main (int argc, char** argv) {
  printf("Starting main\n");
  queue_init(&tempQueue);
  static const char* usage = "usage: aRead numBlocks";
  int numBlocks = 0;
  if (argc == 2)
    numBlocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (numBlocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }

  uthread_init (1);
  disk_start   (interruptServiceRoutine);

  run (numBlocks);
}

