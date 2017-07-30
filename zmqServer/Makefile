all:	zmqServer8081

zmqServer8081:	zmqServer8081.c
	vim zmqServer8081.c
	gcc zmqServer8081.c -lzmq -o zmqServer8081
	./zmqServer8081 /dev/ttyACM0
