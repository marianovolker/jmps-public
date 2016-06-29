#!/bin/sh


#######################################################################################
C_PATH_LOG="/opt/jmps/scripts/logs"
C_MAX_SIZE_LOG=20000000
C_QUANTITY_OF_VIDEO_DEVICES=8
C_QUANTITY_OF_SOCKET_TCP=7
C_QUANTITY_OF_THREADS=$C_QUANTITY_OF_SOCKET_TCP
C_SOCKET_PORT_DEVICE_1=10000
C_SOCKET_PORT_DEVICE_2=10001
C_SOCKET_PORT_DEVICE_3=10002
C_SOCKET_PORT_DEVICE_4=10003
C_SOCKET_PORT_DEVICE_5=10003
C_SOCKET_PORT_DEVICE_6=10005
C_SOCKET_PORT_DEVICE_7=10006
C_MOTION_SERVICE_STARTUP="/etc/init.d/webcam-server start"
C_MOTION_SERVICE_SHUTDOWN="/etc/init.d/webcam-server stop"
C_QUANTITY_OF_DAYS_TO_REMAIN_RECORDING_FILES=80
C_PATH_FOR_VIDEO_DEVICES="/var/www/jmps"
C_OWNER_FOR_RECORDING_FILES="www-data.www-data"
C_FILE_PERMISSIONS_FOR_RECORDING_FILES="777"

#######################################################################################

# Automation section
#######################################################################################
#C_MAILS_DESTINATARIOS_NOTIFICACIONES="marianovolker@gmail.com marianovolker@yahoo.com.ar cesar_carrizo85@hotmail.com fabricioreitano@gmail.com"
#C_MAILS_DESTINATARIOS_NOTIFICACIONES="marianovolker@gmail.com solutionsjmps@yahoo.com.ar cesar_carrizo85@hotmail.com fabricioreitano@gmail.com fachureitano@gmail.com"
#C_ACCOUNT_MAIL_FROM="solutionsjmps@yahoo.com"
#C_ACCOUNT_PASSWD_FROM="jmps0192"
C_MAILS_DESTINATARIOS_NOTIFICACIONES="marianovolker@gmail.com fabricioreitano@gmail.com cesarcarrizo36@gmail.com"
C_ACCOUNT_MAIL_FROM="MarianoVolker@yahoo.com.ar"
C_ACCOUNT_USER_FROM="MarianoVolker"
C_ACCOUNT_PASSWD_FROM="patitolindo0192"
C_PATH_AUTOMATION_LOG="/opt/jmps/scripts/persiana/logs"
C_PATH_AUTOMATION_BIN="/opt/jmps/scripts/persiana"
C_AUTOMATION_BINARY_NAME="./persiana-controller"
C_START_TIME_OF_WORKDAY="8"
C_END_TIME_OF_WORKDAY="19"
C_AUTOMATION_INODE_DEVICE="/dev/ttyUSB0"
C_DEVICE_1="USB <-> Serial"
#######################################################################################

