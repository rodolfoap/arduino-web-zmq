# arduino-web-zmq
A proof of concept on how to forward REST/JSON POST request from a bash webserver to a ZeroMQ queue from a C server to the SERial port, in order to be parsed and processed on an Arduino board.

![Shield view](https://github.com/rodolfoap/arduino-web-zmq/blob/master/img/scshot.jpg)

## The Client

The client8080.bash acts like a common browser client, will send JSON posts to a web server.

```
	$ cat client8080.bash
		#!/bin/bash
		set -x
		curl -H "Content-Type: application/json" -X POST -d $'{"action":"LT","value":"1974"}\n' http://localhost:8080/api/login
		sleep 1
		curl -H "Content-Type: application/json" -X POST -d $'{"action":"HT","value":"2057"}\n' http://localhost:8080/api/login
```

* Requests are sent:

```
	$ ./client8080.bash
		+ curl -H 'Content-Type: application/json' -X POST -d '{"action":"LT","value":"1974"}
		' http://localhost:8080/api/login
		OK: LT/1974
		+ sleep 1
		+ curl -H 'Content-Type: application/json' -X POST -d '{"action":"HT","value":"2057"}
		' http://localhost:8080/api/login
		OK: HT/2057
```

## The Bash Webserver

* The Bash webServer8080 will receive HTTP REST requests over 8080 and forward them to 0mq.
```
	$ ./webServer8080.bash
		 ---> POST /api/login HTTP/1.1
		 ---> User-Agent: curl/7.38.0
		 ---> Host: localhost:8080
		 ---> Accept: */*
		 ---> Content-Type: application/json
		 ---> Content-Length: 31
		 --->
		 ---> {"action":"LT","value":"1974"}
		 ---> POST /api/login HTTP/1.1
		 ---> User-Agent: curl/7.38.0
		 ---> Host: localhost:8080
		 ---> Accept: */*
		 ---> Content-Type: application/json
		 ---> Content-Length: 31
		 --->
		 ---> {"action":"HT","value":"2057"}
```
## The C ZeroMQ queue

* The C zmqServer8081 will receive 0mq messages on 8081 and forward them to /dev/ttyACM0

```
	$ ./zmqServer8081 /dev/ttyACM0
		Received LT 1974
		Received HT 2057

```
## The Arduino target

* The arduino board will receive the requests and process each command.

## Contents
```
.
|-- AnalogReadSerial: 		The Arduino project
|   |-- AnalogReadSerial.ino	Main file
|   |-- README			Some doc
|   |-- initialize_serial.bash	Initialize the serial port... essential!
|   `-- send_data.bash		How to send test data
|-- README			This file
|-- client8080.bash		The browser impersonator
|-- webServer8080.bash		A bash simple webserver, handles a minimalist JSON/REST request
|-- zmqServer			The ZeroMQ server, receiving ZMQ pushes and fwding to a SER port
|   |-- Makefile		Dummy makefile
|   |-- arduino-serial-lib.c	A helpful library, see
|   |-- arduino-serial-lib.h	https://github.com/todbot/arduino-serial .
|   |-- zmqServer8081.c		The server
|   `-- zmqServer8081.py	A python model for tests
`-- zmqServer8081.c		Just a copy for fast reference.
```
![Screen view](https://github.com/rodolfoap/arduino-web-zmq/blob/master/img/scshot2.png)

RodolfoAP (http://ydor.org)
