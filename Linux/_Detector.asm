GLOBAL _ModifiedThread
GLOBAL _ThreadWhichModify



section .text ; makes this executable
	
	_ThreadWhichModify:
		mov byte[rel Sync], 1				; Synchronize the thread, but to early in order to exploit the FIFO property of the processor cache.
		mov byte[rel codeToModify + 1], 1	; Transform 'mov eax, 0' into 'mov eax, 1'.
		ret

	_ModifiedThread:
		cmp byte[rel Sync], 0				; Wait for the synchronisation.
		jz _ModifiedThread					;
		
		;If a DBI is here, or a debug interrupts the detection will occur.
		;In order to detect VM you should add several CPUID here (depending on your processor).

		codeToModify:						; 0B8h is the opcode for mov eax, imm32, here imm32 is 0.
		db 0B8h, 00h, 00h, 00h, 00h			; mov eax, 0
		
		ret

section .data ; Makes this writable and ensure it is not in the same memory page than the code above.
	Sync : db 0
