//Handle the exported functions in both x86 and x64 despite the different linkage rule (with or without '_').
#ifdef WIN32
extern int ModifiedThread();
extern void ThreadWhichModify();
#else //if WIN64
extern int _ModifiedThread();
extern void _ThreadWhichModify();
#define ModifiedThread _ModifiedThread
#define ThreadWhichModify _ThreadWhichModify
#endif 

#include <Windows.h>
#include <stdio.h>
#define NB_TEST 50
BOOLEAN sync = FALSE;



int main(int argc, char** argv) {

	HANDLE hThread;
	DWORD oldProtect;
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//Change the page right to allow both execution and writing.
	if (VirtualProtect(ModifiedThread, si.dwPageSize - 0xF, PAGE_EXECUTE_READWRITE, &oldProtect) == 0) {
		return EXIT_FAILURE;
	}

	//Create a new thread to which will modify the code to execute.
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWhichModify, NULL, 0, NULL);
	if (hThread == NULL) {
		return EXIT_FAILURE;
	}


	//Execute the function that is modified and display its result.
	printf("%d\n",ModifiedThread());

	//Restore the right of the page.
	VirtualProtect(ModifiedThread, si.dwPageSize - 0xF, oldProtect, &oldProtect);
	WaitForSingleObject(ModifiedThread, INFINITE);

	return EXIT_SUCCESS;
}