#!/bin/sh

#######################################################################################
C_JMPS_LIB_FILE="./jmps.lib.sh"
. $C_JMPS_LIB_FILE 2>/dev/null
test $? -ne 0 && echo "ERROR: loading library file, it must located in [$C_JMPS_LIB_FILE]" && exit 1
#######################################################################################

#######################################################################################
debug=0
test $debug -eq 1 && set -x
#######################################################################################

#######################################################################################
# name 			: install_files
# description 	: Copy files to the filesystem.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
install_files()
{
	local rc=0
	
	test $rc -eq 0 && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: fix_actual_path
# description 	: This function fix the actual path of this execution, that means the  
#				  form like the shell script was call, if this is like "./script.sh" then 
#				  the ./ is completed with the full path where the shell script is located.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
fix_actual_path()
{
	local err=0
	local check=""
	local actual_path=$(dirname $0)
	
	if test -z "$actual_path"
	then
		actual_path="./"
	else
		check=$(get_rigth_n_chars "$actual_path" "1")
		test "$check" != "/" && actual_path="$actual_path""/"
	fi
	
	err=$(expr $err + $?)
	
	test $err -eq 0 && printf "$actual_path" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
quit_with_signal()
{
	erase_flag_for_instance_running "$0"
	
	warning "Interrupt signal received, quiting ..."
	log "Interrupt signal received, quiting ..."
	
	log "**********************************************************************************"
	
	exit 0
}
#######################################################################################

#######################################################################################
# name 			: quit
# description 	: Make the initialization.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
quit()
{
	local rc="$1"
	
	erase_flag_for_instance_running "$0"
	log "Normal termination [rc=$rc], quiting ..."
	
	if test $err -eq $C_OK
	then
		log "The execution of [$0] was succefully."
	else
		log "The execution of [$0] was NOT OK."
		err=$C_NOK
	fi
	
	log "**********************************************************************************"
	exit $rc
}
#######################################################################################


#######################################################################################
# name 			: setup
# description 	: Make the initialization.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
setup()
{
	log "**********************************************************************************"
	
	# Trap any signal between 1-30 and call quit function to log and exit cleanly.
	trap quit_with_signal 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30
	
	DIR_WEB_SERVER="/etc/init.d"
	DIR_MOTION_SERVER="/usr/local/bin/motion"
	DIR_JMPS_WEB_SERVER="/var/www/jmps"
	
	check_if_there_is_another_instance_running "$0"
	if test $? -ne $C_OK
	then
		show_error_and_exit "There is another instance of [$0] running."
					
		exit $C_NOK
	fi

	return 0
}
#######################################################################################

#######################################################################################
# name 			: show_error_and_exit
# description 	: Show an error and exit.
# parameters	: the message to show..
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
show_error_and_exit()
{
	local message="$1"

	log "Error: [$message]"
	log "Exiting ..." 
	log "**********************************************************************************"
	
	exit $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: main
# description 	: Main function of the shell script.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
main()
{
	# Initialization ...
	###################################################################################
	setup

	local err=0
	local actual_path=""
	
	actual_path=$(fix_actual_path)
	err=$(expr $err + $?)
	test $err -ne 0 && show_error_and_exit "Building actual path [$actual_path]." 
	###################################################################################	
	

	
	###################################################################################
	# We need to do the following things in the correct order, so please do not change this order.


	###################################################################################


	
	erase_flag_for_instance_running "$0"
	log "**********************************************************************************"
	
	exit $err
}
#######################################################################################

# Execute the main function without parameters...
#######################################################################################
main
#######################################################################################
