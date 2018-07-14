all:	
	gcc -Wall -o RECEIVER receiver.c
	gcc -Wall -o SENDER sender.c
clean:
	rm SENDER RECEIVER

