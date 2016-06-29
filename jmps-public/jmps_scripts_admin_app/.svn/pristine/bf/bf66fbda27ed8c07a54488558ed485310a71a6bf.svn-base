
port=$( ( expr "$1" + 0 2>/dev/null 1>&2 && test "$1" -ge 2 -a "$1" -le 13 && printf "%d" "$1" ) || ( printf "%d" "2" ) )
flag=1
time=0
path=/scripts_jmps
binary=relay_handler

$path/$binary $port $flag $time

exit $?

