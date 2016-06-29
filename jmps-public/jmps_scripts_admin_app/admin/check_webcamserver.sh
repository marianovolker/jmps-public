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
# name 			: quantity_of_threads
# description 	: Quantify the total of threads running by the binary of webcam server (motion).
# parameters	: none.
# return		: Quantity of threads.
#######################################################################################
quantity_of_threads()
{
	local quantity=$(ps aux -T | grep motion | grep -v grep | grep -v motion.pid | wc -l 2>/dev/null)
	echo $quantity
	return $quantity
}
#######################################################################################

#######################################################################################
# name 			: is_quantity_of_threads_equal_to_needed
# description 	: Check if quantity of total threads running at this moment by the  
#				  binary of webcam server is enough.
# parameters	: none.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
is_quantity_of_threads_equal_to_needed()
{
	local quantity=$(quantity_of_threads)
	local total_needed=$(expr $C_QUANTITY_OF_THREADS + 1)
	
	if test $quantity -ge $total_needed
	then
		log "is_quantity_of_threads_equal_to_needed(YES)"
		return $C_TRUE
	else
		log "is_quantity_of_threads_equal_to_needed(NO)"
		return $C_FALSE
	fi
}
#######################################################################################

#######################################################################################
# name 			: is_quantity_of_listen_sockets_equal_to_needed
# description 	: Check if quantity of total sockets used at this moment by the  
#				  binary of webcam server is enough.
# parameters	: none.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
is_quantity_of_listen_sockets_equal_to_needed()
{
	local quantity=0
	local quantity_1=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_1 | wc -l 2>/dev/null)
	local quantity_2=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_2 | wc -l 2>/dev/null)
	local quantity_3=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_3 | wc -l 2>/dev/null)
	local quantity_4=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_4 | wc -l 2>/dev/null)
	local quantity_5=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_5 | wc -l 2>/dev/null)
	local quantity_6=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_6 | wc -l 2>/dev/null)
	local quantity_7=$(netstat -ln -t tcp | grep $C_SOCKET_PORT_DEVICE_7 | wc -l 2>/dev/null)
	quantity=$(expr $quantity_1 + $quantity_2 + $quantity_3 + $quantity_4 + $quantity_5 + $quantity_6 + $quantity_7)
	
	if test $quantity -eq $C_QUANTITY_OF_SOCKET_TCP
	then
		log "is_quantity_of_listen_sockets_equal_to_needed(YES)"
		return $C_TRUE
	else
		log "is_quantity_of_listen_sockets_equal_to_needed(NO:quantity=$quantity;needed=$C_QUANTITY_OF_SOCKET_TCP)"
		return $C_FALSE
	fi
}
#######################################################################################

#######################################################################################
# name 			: is_webcam_server_running
# description 	: Check if the webcam server is running.
# parameters	: none.
# return		: C_TRUE if the event has been ocurred otherwise C_FALSE.
#######################################################################################
is_webcam_server_running()
{
	#----------------------------------------------------------------------------------
	is_quantity_of_listen_sockets_equal_to_needed
	if test $? -eq $C_FALSE
	then
		log "Quantity of listen sockets is less than needed."
		return $C_FALSE
	fi
	#----------------------------------------------------------------------------------
	is_quantity_of_threads_equal_to_needed	
	if test $? -eq $C_FALSE
	then
		log "Quantity of threads is less than needed."
		return $C_FALSE
	fi
	#----------------------------------------------------------------------------------
	
	#----------------------------------------------------------------------------------
	return $C_TRUE
	#----------------------------------------------------------------------------------
}
#######################################################################################

#######################################################################################
# name 			: shutdown_process
# description 	: Do a shutdown for the webcam server process.
# parameters	: none.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
shutdown_process()
{
	local err=0
	
	$C_MOTION_SERVICE_SHUTDOWN >/dev/null 2>&1
	err=$(expr $err + $?)
	sleep 1
	
	quantity_of_threads
	test $? -eq 0 || killall -15 motion
	err=$(expr $err + $?)
	sleep 1
	
	quantity_of_threads
	test $? -eq 0 || killall -9 motion
	err=$(expr $err + $?)
	sleep 1
	
	test $err -eq 0 && return $C_TRUE
	test $err -ne 0 && return $C_FALSE
}
#######################################################################################

#######################################################################################
# name 			: startup_process
# description 	: Startup the webcam server process.
# parameters	: none.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
startup_process()
{
	local err=0
	
	$C_MOTION_SERVICE_STARTUP >/dev/null 2>&1
	err=$(expr $err + $?)

	sleep 6
	
	is_webcam_server_running
	err=$(expr $err + $?)
	
	sleep 6

	test $err -eq 0 && return $C_TRUE
	test $err -ne 0 && return $C_FALSE	
}
#######################################################################################

#######################################################################################
# name 			: main
# description 	: Main function of the shell script.
# parameters	: none.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
main()
{
	#----------------------------------------------------------------------------------
	check_if_there_is_another_instance_running "$0"
	if test $? -ne $C_OK
	then
		log "There is another instance of [$0] running. Exiting..."
		exit $C_NOK
	fi
	#----------------------------------------------------------------------------------	
	
	#----------------------------------------------------------------------------------
	is_webcam_server_running
	if test $? -eq $C_FALSE
	then
		local err=0
		
		log "*****************webcam-server is restarting now!!!*******************"

		shutdown_process
		err=$(expr $err + $?)

		startup_process
		err=$(expr $err + $?)

		if test $err -eq $C_TRUE
		then
			log "webcam-server has been restarted!!!"
		else
			log "failed restarting webcam-server!!!"
		fi
	else
		log "webcam-server is running now!!!"
	fi
	#----------------------------------------------------------------------------------

	#----------------------------------------------------------------------------------
	erase_flag_for_instance_running "$0"
	exit $err
	#----------------------------------------------------------------------------------
}
#######################################################################################

#######################################################################################
main
#######################################################################################

