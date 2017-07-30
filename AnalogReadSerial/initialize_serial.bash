#!/bin/bash
set -x

PORT=/dev/ttyACM0

: Disable AUTO-RESET on Serial connection
stty -F $PORT -hupcl

: Configure port
stty -F $PORT cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
