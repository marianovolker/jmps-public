#!/bin/sh

#######################################################################################
C_JMPS_LIB_FILE="/opt/jmps/scripts/jmps.lib.sh"
. $C_JMPS_LIB_FILE 2>/dev/null
test $? -ne 0 && echo "ERROR: loading library file, it must located in [$C_JMPS_LIB_FILE]" && exit 1
#######################################################################################

#######################################################################################
debug=0
test $debug -eq 1 && set -x
#######################################################################################

#######################################################################################
send_mail()
{
	local subject="$1"
	local body="$2"
	local from="$C_ACCOUNT_MAIL_FROM"
	local user_from="$C_ACCOUNT_USER_FROM"
	local passwd_from="$C_ACCOUNT_PASSWD_FROM"
	local err=0
	local output=""
	local output_total=""
	local mail=""
	
	log "################################################################################"
	log "sendemail -f $from -u $subject -s smtp.mail.yahoo.com -xu $user_from -xp $passwd_from -t $mail -m $body"
	log "################################################################################"
	
	local IFS_OLD=$IFS
	IFS=" "
	for mail in $C_MAILS_DESTINATARIOS_NOTIFICACIONES
	do
		log "[mail: $mail]"
		output=$(sendemail -f $from -u "$subject" -s smtp.mail.yahoo.com -xu $user_from -xp $passwd_from -t "$mail" -m "$body" 2>&1)
		output_total="$output_total""."$output
		err=$(expr $err + $?)
	done
	IFS=$IFS_OLD
	
	log "################################################################################"

	test $err -gt 0 && log "" && log "ERROR: send_mail():[$output_total]" && log ""
	
	
	return $err
}
#######################################################################################

#######################################################################################
check_status_persiana()
{
	local output=$(cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME "status" "0")
	err=$(expr $err + $?)
	log "cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME status 1"
	log "$output"
	
	echo $output
	
	return $err
}
#######################################################################################

#######################################################################################
check_status_puerta()
{
	local output=$(cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME "status" "1")
	err=$(expr $err + $?)
	log "cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME status 0"
	log "$output"
	
	echo $output
	
	return $err
}
#######################################################################################

#######################################################################################
open_persiana()
{
	local err=0
	
	local output=$(cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME "pulse")
	err=$(expr $err + $?)
	log "cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME pulse"
	
	echo $output
	
	return $err
}
#######################################################################################

#######################################################################################
close_persiana()
{
	local err=0
	
	local output=$(cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME "pulse")
	err=$(expr $err + $?)
	log "cd $C_PATH_AUTOMATION_BIN && $C_AUTOMATION_BINARY_NAME pulse"
	
	echo $output
	
	return $err
}
#######################################################################################

#######################################################################################
is_open_persiana()
{
	local output=$(check_status_persiana 2>/dev/null)
	local file=$0
	file=$(basename $file)
	file=$(echo "$C_PATH_AUTOMATION_LOG""/$file"".contador.rec")
	
	if test "$output" = "opened" -o "$output" = "undefined"
	then
		should_be_open
		if test $? -eq 0
		then
			test -f $file && rm -f $file
			return 0
		else
			local MAX_QUANTITY_OF_RETRIES=4
			local contador=0
			local rc=1
	
			if test -f $file
			then
				contador=$(awk -F '=' '{printf "%d", $2}' $file 2>/dev/null)
			
				if test "$contador" -ge "$MAX_QUANTITY_OF_RETRIES"
				then
					log "Luego de 4 o mas, [$contador], intentos informo que la PERSIANA esta abierta!."
					rc=0
				else
					log "Tiene [$contador] intentos de chequeo de apertura, no informo todavia que la PERSIANA esta abierta!."
					rc=1
				fi
			fi

			contador=$(expr $contador + 1)
			echo "contador=$contador" > $file
			return $rc
		fi
	else
		test -f $file && rm -f $file
		return 1
	fi 
}
#######################################################################################

#######################################################################################
is_open_puerta()
{
	local output=$(check_status_puerta 2>/dev/null)
	local file=$0
	file=$(basename $file)
	file=$(echo "$C_PATH_AUTOMATION_LOG""/$file"".door.contador.rec")

	if test "$output" = "opened" -o "$output" = "undefined"
	then
		local MAX_QUANTITY_OF_RETRIES=4
		local contador=0
		local rc=1
	
		if test -f $file
		then
			contador=$(awk -F '=' '{printf "%d", $2}' $file 2>/dev/null)
			
			if test "$contador" -ge "$MAX_QUANTITY_OF_RETRIES"
			then
				log "Luego de 4 o mas, [$contador], intentos informo que la PUERTA esta abierta!."
				rc=0
			else
				log "Tiene [$contador] intentos de chequeo de apertura, no informo todavia que la PUERTA esta abierta!."
				rc=1
			fi
		fi

		contador=$(expr $contador + 1)
		echo "contador=$contador" > $file
		return $rc
	else
		test -f $file && rm -f $file
		return 1
	fi 
}
#######################################################################################

#######################################################################################
is_holiday_date()
{
	local day=$(date +%d)
	return 1
}
#######################################################################################

#######################################################################################
is_laboral_date()
{
	local name_day=$(date +%a)
	
	is_holiday_date
	test $? -ne 0 || return 1 

	log "not a holiday date"

	log "name_day=$name_day"
	
	case "$name_day" in 
		"lun"|"mar"|"mie"|"jue"|"vie")return 0;;
		"Mon"|"Tue"|"Wed"|"Thu"|"Fri")return 0;;
		*)return 1;;
	esac
	
	return 0
}
#######################################################################################

#######################################################################################
should_be_open()
{
	is_laboral_date
	test $? -eq 0 || return 1
	
	local laboral_hour_begin="$C_START_TIME_OF_WORKDAY"
	local laboral_hour_end="$C_END_TIME_OF_WORKDAY"
	local current_hour=$(date +%H)
	
	log "current_hour=$current_hour -ge laboral_hour_begin=$laboral_hour_begin -a current_hour=$current_hour -lt laboral_hour_end=$laboral_hour_end"
	test "$current_hour" -ge "$laboral_hour_begin" -a "$current_hour" -lt "$laboral_hour_end" || return 1
	return 0 
}
#######################################################################################

#######################################################################################
automatic()
{
	test $(id -u) -eq 0 && log "chmod 666 $C_AUTOMATION_INODE_DEVICE" && chmod 666 $C_AUTOMATION_INODE_DEVICE

	is_open_persiana
	if test $? -eq 0
	then
		should_be_open
		if test $? -ne 0
		then
			local C_AUTOMATION_MSJ_NOTIFICATION="LA PERSIANA DEL ESTACIONAMIENTO ESTA ABIERTA EN UN HORARIO NO PERMITIDO"
			local time_stamp=$(date "+%d/%m/%Y_%H:%M:%S")
			send_mail "$C_AUTOMATION_MSJ_NOTIFICATION" "[$time_stamp] ==> [$C_AUTOMATION_MSJ_NOTIFICATION]"
			return $?
		fi
	fi
	
   is_open_puerta
	if test $? -eq 0
	then
		local C_AUTOMATION_MSJ_NOTIFICATION="LA PUERTA DEL ESTACIONAMIENTO ESTA ABIERTA"
		local time_stamp=$(date "+%d/%m/%Y_%H:%M:%S")
		send_mail "$C_AUTOMATION_MSJ_NOTIFICATION" "[$time_stamp] ==> [$C_AUTOMATION_MSJ_NOTIFICATION]"
		return $?
	fi	
	 
	return 0
}
#######################################################################################

#######################################################################################
main()
{
	#----------------------------------------------------------------------------------
	C_PATH_LOG=$C_PATH_AUTOMATION_LOG
	#----------------------------------------------------------------------------------
	
	#----------------------------------------------------------------------------------
	check_if_there_is_another_instance_running "$0"
	if test $? -ne $C_OK
	then
		log "There is another instance of [$0] running. Exiting..."
		exit $C_NOK
	fi
	#----------------------------------------------------------------------------------	

	#----------------------------------------------------------------------------------	
	# Descomentar para realizar pruebas de envio de mails ...
	#
	#local time_stamp=$(date "+%d/%m/%Y_%H:%M:%S")
	#send_mail "PRUEBA ENVIO DE MAIL" "[$time_stamp] ==> [PRUEBA ENVIO DE MAIL]"

	#erase_flag_for_instance_running "$0"
	#exit $C_NOK
	#----------------------------------------------------------------------------------	
	
	#----------------------------------------------------------------------------------
	local err=0
	local action="$1"
	
	case "$action" in
		"automatic")
			automatic
			err=$(expr $err + $?)
		;;
		"open")
			output=$(open_persiana)
			err=$(expr $err + $?)
		;;
		"close")
			output=$(close_persiana)
			err=$(expr $err + $?)		
		;;
		*)
			log "invalid command [$action]!."
			err=1
		;;
	esac 
	#----------------------------------------------------------------------------------

	#----------------------------------------------------------------------------------
	if test $err -eq $C_OK
	then
		log "The execution of [$0] was succefully."
	else
		log "The execution of [$0] was NOT OK."
		err=$C_NOK
	fi
	
	erase_flag_for_instance_running "$0"
	
	exit $err
	#----------------------------------------------------------------------------------	
}
#######################################################################################

#######################################################################################
main $1
#######################################################################################

