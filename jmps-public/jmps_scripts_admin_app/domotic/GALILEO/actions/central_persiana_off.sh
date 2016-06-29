
dispositivo_central_persiana=7

sh /scripts_jmps/digital_port_off.sh "$dispositivo_central_persiana";rc_1=$?

rc=$(expr $rc_1 + 0)

exit $rc
