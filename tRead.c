#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t tempQueue2;


void interruptServiceRoutine () {
    uthread_unblock(queue_dequeue (&tempQueue2));
}

void blockUntilComplete() {
    return;
}

struct ReadArgs {
    char* buf;
    int   nbytes;
    int   blockno;
};

void handleRead (char* buf, int nbytes, int blockno) {
    assert (*((int*)buf) == blockno);
}

void* readAndHandleBlock (void* argsv) {
    struct ReadArgs *RAargs = argsv;
    disk_scheduleRead (RAargs->buf, RAargs->nbytes, RAargs->blockno);
    uthread_block();
    handleRead(RAargs->buf, RAargs->nbytes, RAargs->blockno);
    return NULL;
}

void run (int numBlocks) {
    int temp = 20;
    uthread_t* thread [numBlocks];
    for (int blockno = 0; blockno < numBlocks; blockno++) {
        struct ReadArgs *read = malloc (sizeof(struct ReadArgs));
        read->buf = malloc(numBlocks);
        read->blockno = blockno;
        read->nbytes = temp;
        thread[blockno] = uthread_create (readAndHandleBlock, read);
        queue_enqueue (&tempQueue2, thread[blockno]);
        printf ("buf = %d, blockno = %d\n", *((int*) (read->buf)), (read->blockno));
    }
    for (int i=0; i<numBlocks; i++)
        uthread_join (thread [i]);
}

int main (int argc, char** argv) {
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
