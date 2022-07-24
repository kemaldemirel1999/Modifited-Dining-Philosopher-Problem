DINING PHILISOPHER PROBLEM WITH LIMITED RICE

1)DETAILED INFORMATIONS:

Program takes number of philosopher as an input from command line. It contains differences from the classical dining philosopher problem.There is a waiter who serves rice to the table and there are n-philosophers.The waiter and each philosopher work as a thread.The quantity of rice in the middle is limited and is renewed by the waiter, with a minimum of 1 and a maximum number of philosophers. Even if there are enough portions in the same round, some philosophers may eat more than one (ate, thought, ate again), while others may starve that day if they have no luck. The waiter serves in each 10ms and it represents one day. The program runs for 1000 days and the relevant outputs are displayed on the terminal.

In the second round, first philosopher thread has the priority. The program is run in the same way and the relevant outputs are displayed on the terminal.

RESULT:

Therefore, we have to opportunity to examine the effects of priorization by the operating system among threads doing the same job in the application layer.


2)HOW TO EXECUTE:

gcc modifiedDiningPhilosopher.c -lpthread -o program.o  

-program takes number of philosopher as an input from command line.

./program.o [number of philosopher] 

example to execute executable file: ./program.o 20 



