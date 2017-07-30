#!/bin/bash
set -x

curl -H "Content-Type: application/json" -X POST -d $'{"action":"LT","value":"0734"}\n' http://localhost:8080/api/login
sleep 1
curl -H "Content-Type: application/json" -X POST -d $'{"action":"HT","value":"2057"}\n' http://localhost:8080/api/login
