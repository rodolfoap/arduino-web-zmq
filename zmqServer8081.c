#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "arduino-serial-lib.c"
#include "arduino-serial-lib.h"
#include <string.h>
#include <assert.h>
#define MAX_LINE 256

int main(int argc, char* argv[]) {
	void *context = zmq_ctx_new ();
	void *responder = zmq_socket (context, ZMQ_PULL);
	int rc = zmq_bind (responder, "tcp://*:8081");
	assert (rc == 0);

	int timeout = 10000;

	if (argc == 1) {
		printf("You have to pass the name of a serial port.\n");
		return -1;
	}

	int baudrate = B115200;
	int arduino = serialport_init(argv[1], baudrate);
	if (arduino == -1) {
		printf("Could not open serial port %s.\n", argv[1]);
		return -1;
	}

	sleep(2);
	char line[MAX_LINE];
	while (1) {
		char buffer [128];
		zmq_recv (responder, buffer, 128, 0);
		printf ("Received %s\n", buffer);

		int rc = serialport_write(arduino, buffer);
		if (rc == -1) { printf("Could not write to serial port.\n"); }
		serialport_write(arduino, "\n");
		//zmq_send (responder, "word!", 5, 0);
	}
	serialport_close(arduino);
	return 0;
}
