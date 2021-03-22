# Compilation, debug et gestionnaire de signaux

## Exercise 1: GDB et fichier core

* Question 1: The process finishes with a segmentation fault and the core is dumped. It can be deduced that its end was involuntary because it accessed a memory that it did not own.

* Question 2: The signal produced was SIGSEGV, as informed by gdb. It can also be seen by using *echo $?*, which will display the error 139, representing segmentation fault error.

* Question 3: The problem happens at the gps code, with the usage of *nmea_vtg(&vtg)* at line 40 of the source, which triggers an error at the nmea library at line 23 with the command *puts(NULL)*, being NULL a faulty parameter for puts, as it attends a points for a string.

* Question 4: It doesn't succeed in running the binary as it cannot find the shared library. If the *LD_LIBRARY_PATH* variable is fixed to point to the local *lib/* folder, then it runs the binary and crashes the same way as before, waiting for a command from the user to continue the interaction.

* Question 5: The command *ldd* is used to see which shared libraries are linked, that being useful as multiple versions of a library may be installed in the same computer.

* Question 6: By correcting the library being used for the commands that fail.

* Question 7: The command n, short for next, executes until the next line of code in the source, as long as there are debugging annotations allowing that. The command s, short for step, does the same thing, but doesn't enter functions as they appear in the code, skipping their execution.

* Question 8: Remote tools like that are useful when the code is being run a computer different from the one being used, such as in servers and robots, where there are multiple pieces of hardware to be managed.


## Exercise 2: LD_PRELOAD et sigaction

* Questions 1, 2, 3: in their source files

* Question 4: It is an user command.

* Question 5: The signal handler is located from the line 93 to 97 in the gps.c file. The handler is first initialized as a function that processes the signals being caught, first by displaying it, then closing the connection to the ptmx and finally exiting the process correctly. Afterwards, the set of signals is initialized with nothing (empty set), and the modifiers to the signals is also set to none (0 as the flag represents no modifiers). In the end, the sigaction function is called, changing the process to behave when receiving SIGINT signal as previously specified by the action (and the handler inside).

* Question 6: In code.

* Question 7: One way is to just kill the process, by finding its PID and using the kill tool, such as in *kill 88783*, or with *pkill gps*, being gps the name of the executable running. Another alternative is to suspend it with ctrl-z.

* Question 8: It compiles the libnmea with GPS_OK=1, making it not execute the code in lines 20-24 in nmea.c. It works by changing a flag used inside the code during compilation.


## Exercise 3: Terminal série (minicom) 

* Question 1: Used the command *minicom -p /dev/pts/X*, being X informed when executing gps.

* Question 2: The speed is how many bits per second are transmitted, the parity is used in case any is expected so to have some fault tolerance (it's set to none) and data is the number of bits of each message.

* Question 3: It was used the function "Capture on/off" inside minicom.

