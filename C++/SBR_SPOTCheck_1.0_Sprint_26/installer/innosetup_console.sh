#!/bin/sh

# WARNING: This script is meant to be run ONLY ON PSBS08
# (SPOTCheck's build server) and this is called by the Jenkins's jobs
# author: Sebastian Lipchak
# date: 30 May 2013

#######################################################################################
# name 			: log
# description 	: log a line received by parameter in the console add a new line to each 
#				  line logged.
# parameters	: $1 the line to log.
# return		: 0 if OK, otherwise 1.
# example of use: 
#				log "Hello, this a try."
#				$?  ==> 0  this value is produce by return call.
#######################################################################################
log()
{
   local line_to_log="$1"
   
   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null

   return $?
}
#######################################################################################

unset DISPLAY
scriptname=$1

if test -x /usr/bin/wine 
then 

	[ -f "$scriptname" ] && scriptname=$(winepath -w "$scriptname")
	wine "C:\Program Files\innosetup\ISCC.exe" "$scriptname" "$2" "$3" "$4" "$5" "$6" "$7" "$8" "$9"
	
	if test $? -ne 0
	then
		log  "ERROR Generating installer with InnoSetup."
		exit 1
	fi 
	
	exit 0
	    	 
else
    log  "ERROR Wine not installed."
	exit 2

fi 


