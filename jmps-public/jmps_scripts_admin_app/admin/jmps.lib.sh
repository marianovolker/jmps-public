#!/bin/sh

#######################################################################################
C_JMPS_CONFIG_FILE="/opt/jmps/scripts/jmps.conf.sh"
. $C_JMPS_CONFIG_FILE 2>/dev/null
test $? -ne 0 && echo "ERROR: loading configuration file, it must located in [$C_JMPS_CONFIG_FILE]" && exit 1
#######################################################################################




#######################################################################################
# Constants needed by this lib.
#######################################################################################
C_TRUE=0
C_FALSE=1
C_OK=0
C_NOK=1
export PATH="$PATH:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
#######################################################################################

#######################################################################################
# Global variables needed by this lib.
#######################################################################################
last_update_of_flagfile=0
#######################################################################################

#######################################################################################
# name 			: log
# description 	: log a line received by parameter in the logfile.
# parameters	: $1 the line to log.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
log()
{
   local line_to_log="$1"
   local prefix_to_log=$(date "+%Y/%m/%d_%H:%M:%S:%N:")
   local file=$0
   local size_of_file=0
   file=$(basename $file)
   file=$(echo "$C_PATH_LOG""/$file"".log")

   size_of_file=0
   if test -f $file
   then
      size_of_file=$(stat $file 2>/dev/null | awk -F':' '{if ($1=="  Size") {printf "%s", $2}}' | awk '{printf "%s", $1}' 2>/dev/null)
   fi
   if test $size_of_file -ge $C_MAX_SIZE_LOG
   then
      echo >$file
      echo "$prefix_to_log"" [The logfile has been truncated]" >>$file 2>/dev/null
   fi

   echo "$prefix_to_log"" [$line_to_log]" >>$file 2>/dev/null

   return $C_TRUE
}
#######################################################################################

######################################################################################
# name 			: is_su
# description 	: Check if the user is root.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
is_su()
{
	local rc=$(id -u)

	test $rc -eq 0 && return $C_OK

	return $C_NOK
}
#######################################################################################

######################################################################################
# name 			: execute_action
# description 	: Execute an action and verify if that is ok if not then do retries and so on.
# parameters	: $1: the action to execute.
#				  $2: retries.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
execute_action()
{
	local action="$1"
	local rc=0
	local retries=4
	local output=""
	
	test -n "$2" && retries=$2
	
	log "*******************************************"
	log "execute_action(): action=[$action] retries=[$retries]"

	output=$($action 2>&1); rc=$?; output=$(replace_new_line_in_string "$output" " "); test -n "$output" && log "output: [$output]"
	while test $rc -ne 0 -a $retries -gt 0
	do
		retries=$(expr $retries - 1)
		log "retries = [$retries], rc=[$rc]"
		sleep 1
		output=$($action 2>&1); rc=$?; output=$(replace_new_line_in_string "$output" " "); test -n "$output" && log "output: [$output]"
	done

	if test $rc -eq 0
	then
		log "execute_action(): Success executing action=[$action] rc=[$rc]"
		log "*******************************************"
		return $C_OK
	else
		log "execute_action(): Error executing action=[$action] rc=[$rc]"
		log "*******************************************"
		return $C_NOK
	fi
}
#######################################################################################

#######################################################################################
# name 			: check_entry
# description 	: Check if the entry is a valid inode in the filesystem.
# parameters	: $1 the inode to evaluate.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
check_entry()
{
	local entry="$1"
	
	test -n "$entry" -a -f "$entry" || return $C_FALSE

	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: check_dir_entry
# description 	: Check if the entry is a valid inode in the filesystem as a directory.
# parameters	: $1 the inode to evaluate.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
check_dir_entry()
{
	local entry="$1"
	
	test -n "$entry" -a -d "$entry" || return $C_FALSE

	return $C_TRUE
}
#######################################################################################

#######################################################################################
# name 			: replace_new_line_in_string
# description 	: Replace the character new line in the string for the second parameter.
# parameters	: $1 the string.
#				  $2 the character for the replacement.
# return		: C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
replace_new_line_in_string()
{
	local string="$1"
	local character="$2"
	local new_string=""
	local i=""
	
	for i in $(echo $string)
	do
		new_string="$new_string""$i""$character"
	done

	echo "$new_string"	

	return $C_TRUE
}
#######################################################################################

######################################################################################
# name 			: get_difference_between_two_dates
# description 	: Obtain the difference between two dates in days. The dates must be in
#				  format yyyymmdd.
# parameters	: $1 first date.
#				  $2 second date.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_difference_between_two_dates()
{
	local date_1="$1"
	local date_2="$2"
	local rc=0
	local epoch_date_1=""
	local epoch_date_2=""
	local diference_in_days=0
	
	epoch_date_1=$(date -d "$date_1" "+%s" 2>/dev/null); rc=$(expr $rc + $?)
	epoch_date_2=$(date -d "$date_2" "+%s" 2>/dev/null); rc=$(expr $rc + $?)
	
	if test $rc -eq 0
	then
		if test "$epoch_date_1" -ge "$epoch_date_2" 
		then
			diference_in_days=$((($epoch_date_1 - $epoch_date_2)/86400))
		else
			diference_in_days=$((($epoch_date_2 - $epoch_date_1)/86400))
		fi
		
		rc=$(expr $rc + $?)
	else
		return $C_NOK
	fi

	test $rc -eq 0 && echo "$diference_in_days" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: get_rigth_n_chars
# description 	: Obtain the n quantity of chars from a string begining at right.
# parameters	: $1 the string to parse.
#				  $2 the n value
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_rigth_n_chars()
{
	local string="$1"
	local quantity="$2"
	local rc=0
	local substr=$(awk -v string="$string" -v quantity="$quantity" 'BEGIN{ printf "%s", substr(string, ((length(string)-quantity)+1), quantity) }')
	rc=$(expr $rc + $?)
	
	test $rc -eq 0 && printf "$substr" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: get_inode_name
# description 	: Obtain the inode name from a complete filename with path included 
#				  excluding the extension of it.
# parameters	: $1 the inode to parse.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_inode_name()
{
	local rc=0
	
	local inode="$1"
	
	local dirname=$(dirname $inode); rc=$(expr $rc + $?)
	
	local basename=$(basename $inode); rc=$(expr $rc + $?)
	
	local extension=$(echo "$basename" | awk -F'.' '{printf "%s", $NF}'); rc=$(expr $rc + $?)
	
	local name=$(echo "$basename" | awk -F'.' '{ for(i=1;i<NF;i++){ if(i<(NF-1)){printf "%s%s", $i, FS} else {printf "%s", $i} } }'); rc=$(expr $rc + $?)

	local check=$(get_rigth_n_chars "$dirname" "1"); rc=$(expr $rc + $?)
	
	test "$check" != "/" && dirname="$dirname""/" ; rc=$(expr $rc + $?) 
	
	local inode_name="$dirname""$name"
		
	test $rc -eq 0 && echo "$inode_name" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: get_inode_extension
# description 	: Obtain only the inode extension from a complete filename with path 
#				  included.
# parameters	: $1 the inode to parse.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_inode_extension()
{
	local rc=0
	
	local inode="$1"
	
	local basename=$(basename $inode); rc=$(expr $rc + $?)
	
	local extension=$(echo "$basename" | awk -F'.' '{printf "%s", $NF}'); rc=$(expr $rc + $?)
	
	test $rc -eq 0 && echo "$extension" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: get_time_from_last_inode_modification
# description 	: Obtain the time of the inode from the last modification in minutes. 
# parameters	: $1 the inode to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_time_from_last_inode_modification()
{
	local rc=0
	
	local inode="$1"
	
	check_entry $inode
	test $? -ne $C_OK && log "Bad inode [$inode]" && return $C_NOK
	
	local quantity_of_minutes_from_last_inode_modification=$((($(date "+%s") - $(stat -c "%Y" $inode))/60)); rc=$(expr $rc + $?)	
	
	test $rc -eq $C_OK && echo "$quantity_of_minutes_from_last_inode_modification" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: get_the_pid_of_daemon
# description 	: Obtain the pid of the program or shell script that match with the param.
# parameters	: $1 the string to find in the list of process.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_the_pid_of_daemon()
{
	local inode="$1"
	local rc=0
	local mypid="$$"
	local the_pid_of=""
	local ppid_of_my_ppid=""
	
	ppid_of_my_ppid=$(ps -ef | grep -v grep | awk -v varppid="$PPID" -F' ' '($2==varppid){printf "%s ", $3}' | tr -d '\r' | tr -d '\n')
	
	#Need to exclude my own pid and the parent pid of my own.
	the_pid_of=$(ps -ef | grep "$inode" | grep -v grep | grep -v "$ppid_of_my_ppid" | grep -v "$mypid" | grep -v "$PPID" | awk -F' ' '{printf "%s ", $2}' | tr -d '\r' | tr -d '\n')
	
	rc=$(expr $rc + $?)

	test $rc -eq $C_OK && echo "$the_pid_of" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: check_if_there_is_another_instance_running
# description 	: Verify if there is another instance of the shell script running before.
# parameters	: $1 the inode that represent the shell script to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
check_if_there_is_another_instance_running()
{
	local rc=0
	
	local inode="$1"
	
	local minutes_boundary_to_erase_flagfile=10
	
	local time_last_modif=0
	
	local pid_instance=0
	
	local flag_file=""
	
	local basename=""
	
	check_entry $inode
	test $? -ne $C_OK && log "Bad formatted inode [$inode]" && return $C_NOK

	basename=$(basename $inode); rc=$(expr $rc + $?)	

	flag_file="/tmp/""$basename"".flagfile"; rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && ! test -f "$flag_file" && touch "$flag_file" && return $C_OK
	
	if test -f "$flag_file"
	then
		time_last_modif=$(get_time_from_last_inode_modification "$flag_file")
		
		if test $time_last_modif -gt $minutes_boundary_to_erase_flagfile
		then
			pid_instance=$(get_the_pid_of_daemon "$inode")
			
			log "[my_pid=$$, my_parent_pid=$PPID]"
			
			log "[instance_running ==> [pid=$pid_instance]]"
						
			log "Deleting [flagfile=$flag_file], preventing an execution error, elapsed time [$time_last_modif] minutes..."
			rm -f "$flag_file" 2>/dev/null
			
			log "Killing [shell_script=$inode, pid=$pid_instance]."
			kill -15 "$pid_instance" 2>/dev/null
			
			log "Killing using brutal force of [shell_script=$inode, pid=$pid_instance]."
			kill -9 "$pid_instance" 2>/dev/null	
		fi
	fi

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: erase_flag_for_instance_running
# description 	: Eliminate the flag file that represent the instance of the shell script running.
# parameters	: $1 the inode that represent the shell script to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
erase_flag_for_instance_running()
{
	local rc=0
	
	local inode="$1"
	
	check_entry $inode
	test $? -ne $C_OK && log "Bad formatted inode [$inode]" && return $C_NOK	
	
	local basename=$(basename $inode); rc=$(expr $rc + $?)
	
	local flag_file="/tmp/""$basename"".flagfile"; rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && test -f "$flag_file" && rm -f "$flag_file" && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name 			: touch_flagfile_for_instance_running
# description	: Update the time of the last modification of the flagfile.
# parameters	: $1 the inode that represent the shell script to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
touch_flagfile_for_instance_running()
{
	local rc=0
	
	local inode="$1"
	
	local current_time=$(date "+%s")
	
	test "$last_update_of_flagfile" -eq 0 && last_update_of_flagfile=$current_time
	
	local diff_between_times=$(($current_time-$last_update_of_flagfile))
	
	# if difference between current time and the last update of the flagfile is less
	# than 10 seconds we cancel the touch of the falgfile for performance reason.
	test "$diff_between_times" -le 10 && return $C_OK

	check_entry $inode
	test $? -ne $C_OK && log "Bad formatted inode [$inode]" && return $C_NOK	
	
	local basename=$(basename $inode); rc=$(expr $rc + $?)
	
	local flag_file="/tmp/""$basename"".flagfile"; rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && test -f "$flag_file" && touch "$flag_file" && last_update_of_flagfile=$current_time && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name        	: sizeof
# description	: return the size in bytes of the file.
# parameters	: $1 the file.
# return		: 0 if OK and print the size of the file, otherwise 1.
# example of use:
#					rc$(sizeof ./how_to_build.txt)
#					$?  ==> 0    this value is produce by return call.
#					$rc ==> 5393 this is the size of the file in bytes.
#######################################################################################
sizeof()
{
	local filePath="$1"
	
	local rc=0

	check_entry $filePath
	test $? -ne $C_OK && log "Bad inode [$filePath]" && return $C_NOK

	local sizeBytes=$(stat -c "%s" $filePath 2>/dev/null); rc=$(expr $rc + $?)

	printf $sizeBytes

	test $sizeBytes -ge $C_OK || return $C_NOK

	return $C_OK
}
#######################################################################################

#######################################################################################
# name			: match
# description	: find a pattern in string, if it found then this function returns the 
#				  index in the string where the pattern was found, it is between 1 to n.
# parameters	: $1 the string.
#				  $2 the pattern to find.
# return		: 0 if OK and print the index, otherwise 1.
# example of use: 
#					rc$(match "Where can i find a dog to buy?." "dog")
#					$?  ==> 0  this value is produce by return call.
#					$rc ==> 20 this is the index in the string 1-n
#######################################################################################
match()
{
	local string="$1"
	local pattern="$2"

	local rc=$(awk -v string="$string" -v pattern="$pattern" 'BEGIN{ printf "%d", match(tolower(string), tolower(pattern)); }' 2>/dev/null)
	
	rc=$((($rc + 0)))
	
	test $rc -gt 0 && printf $rc && return $C_OK
	
	return $C_NOK 
}
#######################################################################################

#######################################################################################
# name			: length
# description	: Get the length of the string passed by parameter.
# parameters	: $1 the string.
# return		: 0 if OK and print the len of the string, otherwise 1.
# example of use: 
#					rc$(length "Where ")
#					$?  ==> 0  this value is produce by return call.
#					$rc ==> 6  this is the len of the string.
#######################################################################################
length()
{
	local string="$*"

	local rc=$(awk -v string="$string" 'BEGIN{ printf "%d", length(string); }' 2>/dev/null)
	
	rc=$((($rc + 0)))
	
	test $rc -gt 0 && printf $rc && return $C_OK
	
	return $C_NOK 
}
#######################################################################################

#######################################################################################
# name			: start_progress_indicator
# description	: Create a new process to show a progress indication at console.
# parameters	: $1 time limit.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
start_progress_indicator()
{
	local rc=0
	local time_limit=""
	
	time_limit=$( (test -n "$1" && printf "$1") || (printf "45") )
	
	time_limit=$((($time_limit + 0)))
	time_limit=$( (test "$time_limit" -gt 0 -a "$time_limit" -le 99 && printf "$time_limit") || (printf "45") )
	
	rc=$(expr $rc + $?)
	
	interrupt_progress_bar=1

	local bold=$(tput smso)
	local offbold=$(tput rmso)
	
	rc=$(expr $rc + $?)

	printf " :"
	tput cuf 4
	while test "$time_limit" -gt 0 -a "$interrupt_progress_bar" -eq 1
	do
		tput cub 4
		printf "${bold}[%02d]${offbold}" "$time_limit"
		time_limit=$((($time_limit - 1)))
		sleep 1
	done 2>/dev/null &
	
	rc=$(expr $rc + $?)

	test $rc -eq $C_OK && return $C_OK

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name			: stop_progress_indicator
# description	: Stop the background process that show a progress indication at console.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
stop_progress_indicator()
{
	local rc=0
	
	interrupt_progress_bar=0
	
	kill $! >/dev/null 2>&1
	wait $! >/dev/null 2>&1

	rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && return $C_OK

	return $C_NOK	
}
#######################################################################################

#######################################################################################
# name			: get_current_shell
# description	: Obtain the current shell that this shell script is running.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
get_current_shell()
{
	local rc=0
	
	local current_shell=$(ps -p $$ | grep -v grep | awk '($4=="bash"||$4=="sh"){printf "%s", $4}' 2>/dev/null)

	rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && printf "$current_shell" && return $C_OK
	
	current_shell="bash"

	return $C_NOK
}
#######################################################################################

#######################################################################################
# name			: ask_for_y_or_n
# description	: Ask for a question and wait a configurable time for the response at 
#				  console from the user.  
# parameters	: $1 the message that show for the question.
#				  $2 the configurable time to wait for a response in seconds. The default
#				  time to wait will be set to 45 seconds.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
ask_for_y_or_n()
{
	local legend=""
	local time_limit=""
	local ask=y
	
	legend=$( (test -n "$1" && printf "$1") || (printf "Continue [Y/n]?.") )
	time_limit=$( (test -n "$2" && printf "$2") || (printf "45") )
	
	local current_shell=$(get_current_shell)

	echo
	echo
	tput setf 3
	printf "$legend"
	tput setf 7
	
	start_progress_indicator $time_limit

	if test "$current_shell" = "bash"
	then 
		read -s -t $time_limit -n 1 ask
	else
		read ask
	fi
	
	test -z $ask && ask="y"

	stop_progress_indicator

	echo

	test "$ask" = "y" -o "$ask" = "Y" && return $C_OK
	return $C_NOK
}
#######################################################################################

#######################################################################################
# name			: ask_and_execute
# description	: Ask and wait a configurable time for the response at console from the   
#				  user and then will execute the command passed in the second parameter.  
#				  This function is optimal to execute commands with parameters.
# parameters	: $1 the message that show for the question.
#				  $2 the command to execute
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
ask_and_execute()
{
	local rc=0
	local legend="$1"
	local command="$2"
	local output=""
	shift $#

	if test -n "$command" -a -n "$legend"
	then
		ask_for_y_or_n "$legend [$command]. [Y/n]?: "
		if test $? -eq 0
		then
			output=$(sh -c "$command 2>/dev/null")
			rc=$((($rc + $?)))
			if test "$rc" -ne 0
			then
				log "[output=[$output]]"
			else
				echo "$output"
			fi
		fi
	fi
	return $rc
}
#######################################################################################

#######################################################################################
# name			: ask_and_execute_function
# description	: Ask and wait a configurable time for the response at console from the   
#				  user and then will execute the function passed in the second parameter.  
#				  This function is optimal to execute functions with parameters.
# parameters	: $1 the message that show for the question.
#				  $2 the command to execute
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
ask_and_execute_function()
{
	local rc=0
	local legend="$1"
	local function="$2"
	local output=""
	shift $#

	if test -n "$function" -a -n "$legend"
	then
		ask_for_y_or_n "$legend [$function]. [Y/n]?: "
		if test $? -eq 0
		then
			output=$($function 2>/dev/null)
			rc=$((($rc + $?)))
			test "$rc" -eq 0 &&  echo "$output"
		fi
	fi
	
	return $rc
}
#######################################################################################

#######################################################################################
# name			: warning
# description	: Show a message warning at console in yellow.   
# parameters	: $1 the message to show at console.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
warning()
{
   local line_to_log="$1"

   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo

   tput setf 6
   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null
   tput setf 7

   return $?
}
#######################################################################################

#######################################################################################
# name			: error
# description	: Show a message error at console in red.   
# parameters	: $1 the message to show at console.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
error()
{
   local line_to_log="$1"

   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo

   tput setf 4
   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null
   tput setf 7

   return $?
}
#######################################################################################

#######################################################################################
# name			: success
# description	: Show a message ok at console in green.   
# parameters	: $1 the message to show at console.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
success()
{
   local line_to_log="$1"

   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo

   tput setf 2
   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null
   tput setf 7

   return $?
}
#######################################################################################

#######################################################################################
# name			: fileCopy
# description	: copy a file in a secure form (with retries), this function verify if 
#                 the copy was ok and if there was an error retry the copy until to 
#                 complete the total of retries. This function is useful when is needed 
#                 to copy a file to network filesystem.
# parameters	: $1 file to copy.
#				  $2 directory where the file will be put.
#				  $3 retries.
# return		: C_OK if OK, otherwise C_NOK.
#
# example		:
#				  fileCopy $PATH_SRC/file_test $PATH_TARGET
#				  $?  ==> 0 this value is produce by return call.
#######################################################################################
fileCopy()
{
	local origin="$1"
	local target="$2"
	local retries=$( (test -n "$3" && printf "$3") || echo "4" )

	# Check if the file is readeable by the user that run this script and the original 
	# file has a size greater than zero.
	test -s $origin -a -r $origin || return $C_NOK

	# Check if the target is a directory and if this directory has a write permision 
	# for the user that run this script.
	test -w $target -a -d $target || return $C_NOK

	local i=1
	local rc=0
	local file=$(basename $origin 2>/dev/null)

	cp --force $origin $target 2>/dev/null

	rc=$?

	local sizeOrigin=$(sizeof $origin)
	local sizeTarget=$(sizeof $target/$file)

	while [ $sizeOrigin -ne $sizeTarget ] && [ $i -le $retries ]
	do
		log "Retrying to copy the file [$origin] to [$target/$file]"
		cp --force $origin $target 2>/dev/null
		rc=$?
		sizeTarget=$(sizeof $target/$file)
		i=$(expr $i + 1)
	done

	sizeTarget=$(sizeof $target/$file)
	test $sizeOrigin -ne $sizeTarget && return $C_NOK

	return $C_OK
}
#######################################################################################

#######################################################################################
# name			: check_directory
# description	: Check if the directory passed by parameter exist and is writeable by
#				  the user that run this script.   
# parameters	: $1 the dir to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
check_directory()
{
	local directory=$1

	if [ ! -d "$directory" ] && [ ! -w "$directory" ]
	then
		return $C_NOK
	fi

	return $C_OK
}
#######################################################################################

#######################################################################################
# name			: check_or_create_directory
# description	: Check if the directory passed by parameter exist and is writeable by
#				  the user that run this script or try to create it.   
# parameters	: $1 the dir to verify.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
check_or_create_directory()
{
	local directory=$1
	local rc=0
	
	check_directory "$directory"; rc=$((($rc + $?)))
	test $rc -ne 0 && mkdir -p "$directory" 2>/dev/null && rc=0 && rc=$((($rc + $?)))
	
	if [ $rc -ne 0 ]
	then
		log "ERROR: Unable to locate/create folder [$directory]."
		return $C_NOK
	fi

	return $C_OK
}
#######################################################################################


#######################################################################################
# name			: run_in_background
# description	: Run the actual script in background.
# parameters	: none.
# return		: C_OK if OK, otherwise C_NOK.
#######################################################################################
run_in_background()
{
	local rc=0
	local current_shell=$(get_current_shell)
	local dirname=$(dirname $0)
	local name=$(basename $0)
	local to_execute=""
	local check=""
	local shell_to_execute=""
	
	test "$dirname" = "" -o "$dirname" = "." -o "$dirname" = "./" && dirname="$PWD""/"
	check=$(get_rigth_n_chars "$dirname" "1"); rc=$(expr $rc + $?)
	test "$check" != "/" && dirname="$dirname""/"; rc=$(expr $rc + $?)
	shell_to_execute="$dirname""$name"
	
	to_execute="nohup $current_shell $shell_to_execute $* "' & 2>/dev/null 1>&2; kill $$ 2>/dev/null 1>&2'
	
	nohup $current_shell -c "$to_execute" 2>/dev/null 1>&2 &
	wait $! 2>/dev/null 1>&2

	rc=$(expr $rc + $?)
	
	test $rc -eq $C_OK && return $C_OK
	
	return $C_NOK
}
#######################################################################################
