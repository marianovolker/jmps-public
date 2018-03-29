#!/bin/sh

port_one=21300
port_two=21350

OPTIND=1
while getopts "p:q:h" opt; do
  case "$opt" in
    p)
      port_one=$OPTARG
      ;;
    q)
      port_two=$OPTARG
      ;;
    h)
      cat << EOF
Usage: ${0##*/} [ -h | [ -p first-port ] [ -q second-port ] ]
Provides a port binding between first-port and second-port, opening both as incomming connection servers.
	-p FIRSTPORT 	First port to listen for a connection. Default is 5300.
	-q SECONDPORT	Second port to listen for a connection. Default is 5301.
	-h           	Prints this help.
EOF
      exit 0
      ;;
    \?)
      exit 1
      ;;
  esac
done
shift "$((OPTIND-1))"

echo "\nStarted ${0##*/} at ports ${port_one} and ${port_two}.\n" && trap 'echo "\nClosing ${0##*/}..." && rm -f /tmp/ucc-pipe' INT && { test -p /tmp/ucc-pipe && rm /tmp/ucc-pipe || true ; } && mkfifo /tmp/ucc-pipe && { { nc -k -l ${port_one} < /tmp/ucc-pipe | nc -k -l ${port_two} > /tmp/ucc-pipe ; } || true ; } && rm -f /tmp/ucc-pipe

exit $?
