# EMBSYS

This repository store my lectures on Linux for embedded systems with some
reminders in system programming.

## Lectures

Lectures are separated in several parts:

  * Introduction  licenses, POSIX, opensource vs free software, ... (lect1)
  * Linux System Programming : processus, pthread, signals, mutex, IPC, ...
    (lect2 and lect3)
  * Kernel architectures and Linux module programming (lect4)
  * Bus and communication devices : PCI, I2C, RS232, ... (lect5)

## Labs

Several exercises are available :

  * Linux System Programming (labs1 and labs2)
  * Filesystem discovery as root (labs3 and labs4)

## Dependancies

To compile README from labs directories in PDF, you'll need the a2x binary :

    apt-get install asciidoc texlive-lang-cyrillic

Depending of what backend you wanna use for dblatex, you maybe will need :

    apt-get install fop
