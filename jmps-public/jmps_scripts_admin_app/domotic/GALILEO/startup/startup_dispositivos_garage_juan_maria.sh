
pc_web_cam_port=2
pc_svn_server_port=3
dispositivos_internet=4
dispositivo_grupo=8
dispositivo_central_persiana=7

sh /scripts_jmps/digital_port_on.sh "$pc_web_cam_port";rc_1=$?
sh /scripts_jmps/digital_port_on.sh "$pc_svn_server_port";rc_2=$?
sh /scripts_jmps/digital_port_on.sh "$dispositivos_internet";rc_3=$?
sh /scripts_jmps/digital_port_off.sh "$dispositivo_grupo";rc_4=$?
sh /scripts_jmps/digital_port_on.sh "$dispositivo_central_persiana";rc_5=$?

rc=$(expr $rc_1 + $rc_2 + $rc_3 + $rc_4 + $rc_5)

exit $rc
