# Autotools

* Question 1: The header located in *sys/socket.h* is verified.

* Question 2: Added the following code: *AC_CHECK_HEADER(sys/verycool.h, [],
                [AC_MSG_ERROR(["Couldn't find the cool header"])])*
                
* Question 3: The macro is used in order to be able to create shared libraries.

* Question 4: Makefile.am is a configuration file used by the automake to generate the Makefile.in

* Question 5: One just has to change the a.out values to miniserver inside src/miniserver/Makefile.am

* Question 6: 
