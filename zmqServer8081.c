// Compile with:
// 	gcc zmqServer8081.c -lzmq -o zmqServer8081

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
	void *context = zmq_ctx_new ();
	void *responder = zmq_socket (context, ZMQ_PULL);
	int rc = zmq_bind (responder, "tcp://*:8081");
	assert (rc == 0);

	while (1) {
		char buffer [128];
		zmq_recv (responder, buffer, 128, 0);
		printf ("Received %s\n", buffer);
		sleep (1);
		zmq_send (responder, "word!", 5, 0);
	}
	return 0;
}
