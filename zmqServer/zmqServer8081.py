#!/usr/bin/python

import sys
import zmq
import time

context = zmq.Context()
sock = context.socket(zmq.PULL)
sock.bind("tcp://*:8081")

while True:
    message=sock.recv()
    print message
