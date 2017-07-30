#!/bin/bash

[ -z "$1" ] && { echo "Usage: $0 [SPEED]"; exit; }

PORT=/dev/ttyACM0
printf "$1"'\n' > $PORT
