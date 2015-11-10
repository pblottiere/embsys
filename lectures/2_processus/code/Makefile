all:
	gcc pid.c -o pid
	gcc fork.c -o fork
	gcc signals.c -o signals
	gcc sigaction.c -o sigaction
	gcc thread.c -o thread -pthread
	gcc maxthread.c -o maxthread -pthread
	gcc unlthread.c -o unlthread -pthread

clean:
	rm -f pid fork signals sigaction thread maxthread unlthread
