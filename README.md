# Dynamic analysis detector

A way to detect all DBI frameworks and Debuggers.

This project gives a code sample that allow the detection of DBI and debug interrupt. The method shows here exploit the FIFO (First In First Out) property of the processor cache to detect abnormal cache actualisation. These abnormal actualisation are unintentionally triggers by Dynamic Binary Instrumentation framework (DBI), debug interrupts as well as Virtual Machines (VM).

## Prerequisites

In order to run this project you need [Visual Studio 2019](https://visualstudio.microsoft.com/vs/) installed and ready to run.
You also need [NASM](https://www.nasm.us) installed and in your PATH (follow the [official tutorial](https://www.nasm.us/xdoc/2.09.04/html/nasmdoc1.html)). 

Note that for the rest of this README, PATH_TO_README will refer to the absolute path of this file. Moreover, we will use the file located in the Windows folder.


## Installing

In order to build this project open it with Visual Studio 2019.
Then, delete the "./Detector/_Detector.obj" file if it exists.
Choose a build configuration and target (for instance **Release** and **x64**).
Click on the green arrow (Local Windows Debugger).
Normally the program should print 0.

## Testing

### Control experiment

In order to test this project when build in **Release** mode for **x64** architecture first open a cmd in "./Detector" and run :
```
./x64/Release/Detector.exe
```
It should display 0.

### Pin Detection

To detect [PIN](https://software.intel.com/content/www/us/en/develop/articles/pin-a-dynamic-binary-instrumentation-tool.html), first install it. Then go in the PIN folder and run :
```
pin -- PATH_TO_README\Detector\x64\Release\Detector.exe
```
It should display 1.

### Windbg Preview Time Travel Debugging Detection

To use the [WinDbg Preview](https://www.microsoft.com/fr-fr/p/windbg-preview/9pgjgd53tn86?rtc=1#activetab=pivot:overviewtab) Time Travel Debugging (TTD) follow [this tutorial](https://blogs.windows.com/windowsdeveloper/2017/09/27/time-travel-debugging-now-available-windbg-preview/). Then, use the TTD on the file "./x64/Release/Detector.exe". The program should crash.

### Windbg Preview interrupts Detection

To detect a debugger interrupt issued using WinDbg Preview. First, open WinDbg Preview, then Launch PATH_TO_README\Detector\x64\Release\Detector.exe from WinDbg Preview. Then run the following command inside WinDbg Preview :
```
bp Detector!ModifiedThread + 9
```
It should put a breakpoint at the 'mov eax, 0' located in ./Detector/_Detector.asm
Then run the following command inside WinDbg Preview :
```
g
```
It should stop at the 'mov eax, 0' which is now a 'mov eax, 1'.
Then run the following command a second time inside WinDbg Preview :
```
g
```
It should display 1.

Note that this method exploits the processor interruption mechanism. Thus, it will detect breakpoint and trap flag exception in any debugger.


## For VM

In order for this program to also detect VM you should modify the file "./Detector/_Detector.asm" and add CPUID depending on your processor. Note that there is a method to determine the amount of CPUID on-the-fly, but which is not given here.

## For Linux

A sample to detect VM on Linux guest, DBI and Debuggers is given in the Linux folder. To compile it you just need to run 'RUNME64.sh' or 'RUNME32.sh' depending on your architecture (x64 or x86).
Also make sure that you have 'nasm' and 'gcc-multilib' installed :
```
sudo apt update
sudo apt-get install gcc-multilib g++-multilib
sudo apt-get install nasm
```

## WARNING

Do not forget to have at least 2 CPU cores on your computer and VM.


