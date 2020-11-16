#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

extern int _ModifiedThread();
extern void _ThreadWhichModify();
#define ModifiedThread _ModifiedThread
#define ThreadWhichModify _ThreadWhichModify

#if __x86_64__
typedef unsigned long long uintptr;
#else
typedef unsigned long uintptr;
#endif

int main(int argc, char** argv) {
    pthread_t pthread;
    long page_alignment;
    long pagesize = sysconf(_SC_PAGE_SIZE);
    if(pagesize == -1){
        handle_error("sysconf");
    }

    page_alignment = ~(pagesize-1);

    // Change the page right to allow both execution and writing.
    // Just hope that ModifiedThread function is written in 1 page of memory only.
    if(mprotect( (void*) ((uintptr)(ModifiedThread) & page_alignment), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        handle_error("mprotect");
    }

    // Create the modifying thread.
    if(pthread_create(&pthread, NULL, (void*(*)(void *))ThreadWhichModify, NULL)) {
        handle_error("pthread_create");
    }
    
    //Execute the function that is modified and display its result.
    printf("%d\n", ModifiedThread());

    return 0;
}