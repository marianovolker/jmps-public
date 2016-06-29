
dispositivo_grupo=8

sh /scripts_jmps/digital_port_pulsation.sh "$dispositivo_grupo";rc_1=$?

rc=$(expr $rc_1 + 0)

exit $rc
