#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int mprotect(void* addr, size_t len, int prot);
int pthread_create(pthread_t* thread, pthread_attr_t* attr, void* (*start_routine) (void*), void* arg);

extern int _ModifiedThread();
extern void _ThreadWhichModify();
#define ModifiedThread _ModifiedThread
#define ThreadWhichModify _ThreadWhichModify

#define	PROT_READ	0x04	/* pages can be read */
#define	PROT_WRITE	0x02	/* pages can be written */
#define	PROT_EXEC	0x01	/* pages can be executed */
#define PAGE_SIZE 	4096

#if __x86_64__
typedef unsigned long long uintptr;
#else
typedef unsigned long uintptr;
#endif

int main() {
	pthread_t pthread;

	//Change the page right to allow both execution and writing.
	if(mprotect((void*)(((uintptr) ModifiedThread)&(~0XFFF)), PAGE_SIZE+100, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
		return -1;
	}


	// Create the modifying thread.
	if(pthread_create(&pthread, NULL, (void*(*)(void *))ThreadWhichModify, NULL)) {
		return -1;
	}

	
	//Execute the function that is modified and display its result.
	printf("%d\n", ModifiedThread());

	return 0;
}













