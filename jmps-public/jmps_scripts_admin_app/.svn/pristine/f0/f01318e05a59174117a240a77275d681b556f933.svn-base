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
# name 			: get_year
# description 	: Get the year of the string with a format like this "00B800017D78(002fgyb)_1_20111225121640_66391.jpg".
#				  "nnnnnnnnnnnn_n_yyyymmddhhMMSS_nnnnn.jpg"
# parameters	: $1 the string to split and get the year.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
get_year()
{
	local entry="$1"
	
	local year=$(echo "$entry" | awk -F'_' '{printf "%s", $3}' | cut -c1-4)
	
	echo $year
	
	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: get_month
# description 	: Get the month of the string with a format like this "00B800017D78(002fgyb)_1_20111225121640_66391.jpg".
#				  "nnnnnnnnnnnn_n_yyyymmddhhMMSS_nnnnn.jpg"
# parameters	: $1 the string to split and get the month.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
get_month()
{
	local entry="$1"
	
	local month=$(echo "$entry" | awk -F'_' '{printf "%s", $3}' | cut -c5-6)
	
	echo $month
	
	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: get_day
# description 	: Get the day of the string with a format like this "00B800017D78(002fgyb)_1_20111225121640_66391.jpg".
#				  "nnnnnnnnnnnn_n_yyyymmddhhMMSS_nnnnn.jpg"
# parameters	: $1 the string to split and get the day.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
get_day()
{
	local entry="$1"
	
	local day=$(echo "$entry" | awk -F'_' '{printf "%s", $3}' | cut -c7-8)
	
	echo $day
	
	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: organize_image_files
# description 	: Organize files into directories ordered by day in the year.
#				  At this function we get the day, month and year of these files and
#				  create a new dir with the form ddmmyyyyjj and then move the files
#				  to the created directory.
# parameters	: $1 the entry as directory that represent the recording files of a video
#				  device.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
organize_image_files()
{
	local dir="$1"
	local filter="*.jpg"	
	local err=0
	local file=""
	local new_dir=""
	local y=""
	local m=""
	local d=""
	local j=""
	local size=0
	
	filter=$dir/$filter

	for entry in $filter
	do
		if test -f $entry
		then
			check_entry $entry
			test $? -eq $C_FALSE && log "Discarding [1][$entry]" && continue
			
			echo "$entry" | grep -q "snapshot" 2>/dev/null 1>&2
			test $? -eq $C_TRUE && log "Discarding [2][$entry]" && continue
			
			echo "$entry" | grep -q "lastsnap" 2>/dev/null 1>&2
			test $? -eq $C_TRUE && log "Discarding [3][$entry]" && continue			
			
			y=$(get_year  $entry)
			m=$(get_month $entry)
			d=$(get_day   $entry)
			j=$(date -d "$y$m$d" +%j)

			if test -z "$y" -o -z "$m" -o -z "$d"
			then
				log "Discarding [4][$entry]"
				log "Deleting the inode [$entry].[rm -f \"$entry\"]"
				rm -f "$entry"
				continue
			fi
			
			size=$(sizeof "$entry")
			if ! test "$size" -gt 0
			then
				log "Discarding [5][the file has zero size][$entry]"
				log "Deleting the inode [$entry].[rm -f \"$entry\"]"
				rm -f "$entry"				
				continue
			fi			

			file=$(basename $entry)
			err=$(expr $err + $?)

			new_dir="$dir""/$d""$m""$y""_$j"
			test -d "$new_dir" || ( log "mkdir $new_dir" && mkdir $new_dir )
			err=$(expr $err + $?)

			mv $entry "$new_dir""/$file"
			err=$(expr $err + $?)

			log "mv $entry ""$new_dir""/$file"
			err=$(expr $err + $?)
		else
			log "Discarding [6][$entry]"
		fi 		
	done
	
	chown -R $C_OWNER_FOR_RECORDING_FILES $dir 2>/dev/null
	chmod -R $C_FILE_PERMISSIONS_FOR_RECORDING_FILES $dir 2>/dev/null

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
	check_if_there_is_another_instance_running "$0"
	if test $? -ne $C_OK
	then
		log "There is another instance of [$0] running. Exiting..."
		exit $C_NOK
	fi
		
	local err=0
	local index=1
	local camera_dir=""
	
	while test $index -le $C_QUANTITY_OF_VIDEO_DEVICES
	do
		camera_dir="$C_PATH_FOR_VIDEO_DEVICES""/cam""$index"
		
		log "organize_image_files $camera_dir"
		organize_image_files "$camera_dir"
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
	
	erase_flag_for_instance_running "$0"
	
	exit $err	
}
#######################################################################################


#######################################################################################
main
#######################################################################################
