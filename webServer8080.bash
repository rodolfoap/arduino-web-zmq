#!/bin/bash

# See https://stackoverflow.com/questions/26455434/create-a-minimum-rest-web-server-with-netcat-nc
HTTP_200="HTTP/1.1 200 OK"
HTTP_404="HTTP/1.1 404 Not Found"
HTTP_LOCATION="Location:"

# This receives requests from the browser, at 8080
WEBPORT=8080

# And routes all requests to 0mq, at 8081
MSGPORT=8081

trap "rm -f fifo" EXIT
rm -f fifo
mkfifo fifo

zmq_push () { m=$(cat) && echo -e $(printf '\\x01\\x00\\x%02x\\x00%s' $((1 + ${#m})) "$m") | nc localhost $MSGPORT &>/dev/null; }
# Example usage: echo Hello|zmq_push

json_parse() { m=$(cat) && echo "$m"|sed -r 's/^(.*)"('$1')":"([^"]*)"(.*)$/\3/g'; }
# Example usage: echo '{"action":"fw","value":"13"}'|json_parse action

while true; do cat fifo|nc -l $WEBPORT > >(
	export REQUEST=
	export ACTION=
	export VALUE=
	sleep 0.1
	while read LINE; do
		echo " ---> $LINE"
		LINE=$(echo "$LINE" | tr -d '[\r\n]')
		echo "$LINE"|grep -qE '^GET /'	&& { REQUEST=$(echo "$LINE"|awk '{print $2}'); }
		echo "$LINE"|grep -qE '^{'	&& {
			ACTION=$(echo "$LINE"|json_parse action)
			VALUE=$( echo "$LINE"|json_parse value)
			echo "$ACTION $VALUE"|zmq_push;
			printf "%s\n%s %s\n\n%s\n" "$HTTP_404" "$HTTP_LOCATION" $ACTION "OK: $ACTION/$VALUE" > fifo
		}
	done
); done
