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
# name 			: get_day_of_year
# description 	: Get the day of the year as the julian day (Current day in the year).
# parameters	: $1 the string from the directory to split and get the day of the year.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
get_day_of_year()
{
	local entry="$1"
	
	local day=$(echo "$entry" | awk -F'_' '{printf "%s", $2}')
	
	echo $day
	
	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: get_date_of_entry
# description 	: Get the date of the entry in format yyyymmdd.
# parameters	: $1 the entry in format ddmmyyyy_jjj.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
get_date_of_entry()
{
	local entry=$(basename "$1")
	
	local date=$(echo "$entry" | awk -F'_' '{printf "%s", $1}')
	local yy=$(echo "$date" | cut -c5-8)
	local mm=$(echo "$date" | cut -c3-4)
	local dd=$(echo "$date" | cut -c1-2)
	
	echo "$yy$mm$dd"
	
	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: check_directories_and_erase_old_dirs
# description 	: Check if there are dirs to remove, if the dir is older than 
#				  C_QUANTITY_OF_DAYS_TO_REMAIN_RECORDING_FILES constant then the file will
#				  be deleted. 
# parameters	: $1 the entry as directory that represent the recording/image files of
#				  a video device.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
check_directories_and_erase_old_dirs()
{
	local dir="$1"
	local err=0
	local boundary_day=$C_QUANTITY_OF_DAYS_TO_REMAIN_RECORDING_FILES
	local current_date=$(date "+%Y%m%d")
	local date_of_entry=""
	local difference=""
	
	for entry in $dir/*
	do
		if test -d $entry
		then
			check_dir_entry $entry
			test $? -ne 0 && log "Discarding [$entry]" && continue

			date_of_entry=$(get_date_of_entry $entry)

			difference=$(get_difference_between_two_dates "$current_date" "$date_of_entry")
			
			log "[$entry][current_date=$current_date][date_of_entry=$date_of_entry][diff_in_days=$difference]"
			
			if test "$difference" -gt "$boundary_day"
			then
				log "the dir [$entry] is going to be eliminated."
				rm -rf $entry
				err=$(expr $err + $?)
			else
				log "the dir [$entry] is going to be ignored."
			fi
		fi 		
	done

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
	log "**********************************************************************************"
	check_if_there_is_another_instance_running "$0"
	if test $? -ne $C_OK
	then
		log "There is another instance of [$0] running. Exiting..."
		log "**********************************************************************************"
		exit $C_NOK
	fi

	local err=0
	local index=1
	local camera_dir=""
	
	while test $index -le $C_QUANTITY_OF_VIDEO_DEVICES
	do
		camera_dir="$C_PATH_FOR_VIDEO_DEVICES""/cam""$index"
		
		log "check_directories_and_erase_old_dirs $camera_dir"
		check_directories_and_erase_old_dirs "$camera_dir"
		err=$(expr $err + $?)
		
		log "*"
			
		index=$(expr $index + 1)
	done
		
	if test $err -eq $C_OK
	then
		log "The execution of [$0] was succefully."
	else
		log "The execution of [$0] was NOT OK."
		err=$C_NOK
	fi
	log "**********************************************************************************"
	
	erase_flag_for_instance_running "$0"
	
	exit $err
}
#######################################################################################


#######################################################################################
main
#######################################################################################
