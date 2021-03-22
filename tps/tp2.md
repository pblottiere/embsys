# Compilation, debug et gestionnaire de signaux

* Question 1: The process finishes with a segmentation fault and the core is dumped. It can be deduded that its end was involuntary.

* Question 2: The signal produced was SIGSEGV.

* Question 3: The problem happens at the gps code, with the usage of *nmea_vtg(&vtg)* at line 40 of the source, which triggers an error at the nmea library at line 23 with the command *puts(NULL)*, being NULL a faulty parameter for puts.

* Question 4: It runs the binary and crashes the same way as before, waiting for a command from the user to continue the interaction.

* Question 5: The command *ldd* is used to see which shared libraries are linked, which is useful as multiple versions of the same may be installed in the same computer.

* Question 6: By correcting the library being used for the commands that fail.

* Question 7: The command n, short for next, executes until the next line of code in the source, as long as there are debugging annotations allowing that. The command s, shot for step, does the same thing, but doesn't enter functions as they appear in the code, skipping their execution.

* Qustion 8: Remote tools like that are useful when the code is being run a computer different from the one being used, such as in servers and robots, where there are multiple pieces of hardware to be managed.

