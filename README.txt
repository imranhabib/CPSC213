Imran Habib 
q1n7 (76786094) 


*For the assignment I used a Windows 8 dual-core i5 machine. 

Question 1: 


Table 1: 
SRead tests/execution times: 

sRead 10	: 0.126 seconds  (126ms) 
sRead 100 	: 1.080 seconds  (1080ms) 
sRead 1000 : 10.814 seconds (10814ms) 



The performance times are due to the fact our disk takes about 10ms(milliseconds) to process
a single read request. In the SRead program, the process runs in a synchronous fashion (i.e sequentially)
meaning the processor has to wait for the previous disk read to complete before processing the next scheduled disk read. 
In the case of 10 reads, I would expect the time for a synchronous program to be around 10ms*10 (100ms). My real time estimation was around what I would expect 
from 10 reads (real time: 0.126 seconds == 126ms). 

Question 2: 

Table 2: 
ARead tests/excecution times: 				

ARead 10	: 0.016 seconds (16ms)
ARead 50 	: 0.018 seconds (18ms)
ARead 500 	: 0.020 seconds (20ms) 
ARead 1000 : 0.023 seconds (23ms)


WHAT: Comparing the times revealed from the SRead tests in table 1 (in question 1) with the ARead times in table 2, I have witnessed a drastic decrease in execution times for inputs (above an input of 1) into ARead. 
For an input of 10, SRead took 126ms, while ARead was approx. 10 times faster measuring at 16ms for an input of 10. A similar pattern emerges when you input 1000 into each program, however; the degree in which ARead executes is increased to approx 470 times faster the speed of SRead.  

WHY: In the asynchronous read program (ARead) the scheduled disk reads are interleaved throughout execution which reduces (and may even fully eliminate in ideal circumstances) the waiting time between disk reads. 
In ARead, when a disk read is initialized, the processor moves to execute the next scheduled task (another disk read) while the initial disk read is reading from the disk. The initial disk read (task) will continue until it explicitly tells the processor it is finished by sending an interrupt through the interruptServiceRoutine() function in ARead. To simulate that the waiting should occur AFTER all the scheduled disk reads are executed, the disk_waitForReads() function is placed outside the for-loop, in contrast to its position after each call to disk_scheduleRead() in SRead (in SRead its placed directly after each call). 
This interleaved method of task execution in ARead, is the fundamental reason it results in faster execution times for each test input than the execution times for SRead. 


Question 3 

Table 3
TRead tests/execution times: 

TRead 10:		real: 0.041s user: 0.006s sys: 0.019s 	 
TRead 100:  	real: 0.082s user: 0.008s sys: 0.075s	 
TRead 500: 	real: 0.335s user: 0.010s sys: 0.312s  
TRead 1000:	real: 0.611s user: 0.012s sys: 0.600s 	  

Comparing TRead's execution times (system times)in table 3 to AReads execution time in table 2, it's revealed that TRead is much slower than ARead as input values increase. This result isn't surprising due to the fact TRead is basically SRead (a sequentially executing process that implements blocking) with threads. 

With each thread creation there happens to be memory overhead due to the need for a transfer of state from one thread to another. If the transfer rate is relatively high, which would be expected in a program with several threads and a large amount of tasks, the overhead consequences may outweight the benefits of using threads in the first place. 
