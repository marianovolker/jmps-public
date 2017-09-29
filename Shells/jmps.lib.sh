#!/bin/sh


#######################################################################################
# Constants needed by this lib.
#######################################################################################
C_VERSION="1.1.0.1"
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
# name        : log
# description : log a line received by parameter in the logfile.
# parameters  : $1 the line to log.
# return      : C_TRUE if OK, otherwise C_FALSE.
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
   if test "$size_of_file" -ge "$C_MAX_SIZE_LOG"
   then
      echo >$file
      echo "$prefix_to_log"" [The logfile has been truncated]" >>$file 2>/dev/null
   fi

   echo "$prefix_to_log"" [$line_to_log]" >>$file 2>/dev/null

   return $C_TRUE
}
#######################################################################################

#######################################################################################
# name        : debugging_log_function_in
# description : log a line that indicate the begining of the function. Thast's for
#               debbuging purpose.   
# parameters  : $1 the name of the function.
# return      : C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
debugging_log_function_in()
{
  local functionName=$( (test -n "$1" && printf "$1") || (printf "uname_function_in") )
  
  test $C_DEBUG -eq 2 && log "function___"$functionName"___in():"
  
  return 0
}
#######################################################################################

#######################################################################################
# name        : debugging_log_function_out
# description : log a line that indicate the begining of the function. Thast's for
#                 debbuging purpose.
# parameters  : $1 the name of the function.
# return      : C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
debugging_log_function_out()
{
  local functionName=$( (test -n "$1" && printf "$1") || (printf "uname_function_out") )
  
  test $C_DEBUG -eq 2 && log "function___"$functionName"___out():"
  
  return 0
}
#######################################################################################

#######################################################################################
# name        : is_su
# description : Check if the user is root.
# parameters  : none.
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
is_su()
{
  local rc=$(id -u)

  test $rc -eq 0 && return $C_OK

  return $C_NOK
}
#######################################################################################

######################################################################################
# name        : execute_action
# description : Execute an action and verify if that is ok if not then do retries and so on.
# parameters  : $1: the action to execute.
#               $2: retries.
# return      : C_OK if OK, otherwise C_NOK.
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

  output=$($action 2>&1); rc=$?; output=$(replace_new_line_in_string "$output" ""); test -n "$output" && log "output: [$output]"
  while test $rc -ne 0 -a $retries -gt 0
  do
    retries=$(expr $retries - 1)
    log "retries = [$retries], rc=[$rc]"
    my_sleep 0.7
    output=$($action 2>&1); rc=$?; output=$(replace_new_line_in_string "$output" ""); test -n "$output" && log "output: [$output]"
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
# name        : check_entry
# description : Check if the entry is a valid inode in the filesystem.
# parameters  : $1 the inode to evaluate.
# return      : C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
check_entry()
{
  local entry="$1"

  test -n "$entry" -a -f "$entry" || return $C_FALSE

  return $C_TRUE
}
#######################################################################################

#######################################################################################
# name        : check_dir_entry
# description : Check if the entry is a valid inode in the filesystem as a directory.
# parameters  : $1 the inode to evaluate.
# return      : C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
check_dir_entry()
{
  local entry="$1"

  test -n "$entry" -a -d "$entry" || return $C_FALSE

  return $C_TRUE
}
#######################################################################################

#######################################################################################
# name        : replace_new_line_in_string
# description : Replace the character new line in the string for the second parameter.
# parameters  : $1 the string.
#               $2 the character for the replacement.
# return      : C_TRUE if OK, otherwise C_FALSE.
#######################################################################################
replace_new_line_in_string()
{
  local string="$1"
  local character="$2"
  local new_string=""
  local i=""

  if test -n "$character"
  then
    new_string=$(echo "$string" | tr '\n\r' "$character" 2>/dev/null)
  else
    new_string=$(echo "$string" | tr -d '\n\r' 2>/dev/null)
  fi

  echo "$new_string"	

  return $C_TRUE
} 
#######################################################################################

######################################################################################
# name        : get_seconds_from_time
# description : Obtain the seconds from time in format HH:MM:SS.
# parameters  : $1 hour in format HH:MM:SS.
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
get_seconds_from_time()
{
  local time="$1"
  
  local hour=$(echo "$time" | awk -F ':' '{printf "%02d", $1}')
  local minutes=$(echo "$time" | awk -F ':' '{printf "%02d", $2}')
  local seconds=$(echo "$time" | awk -F ':' '{printf "%02d", $3}')
  
  test "$hour" -lt 0 -o "$hour" -ge 24 && log "The hour [$hour] specificated is not valid ..." && return 1
  test "$minutes" -lt 0 -o "$minutes" -ge 60 && log "The minutes [$minutes] specificated is not valid ..." && return 1  
  test "$seconds" -lt 0 -o "$seconds" -ge 60 && log "The seconds [$seconds] specificated is not valid ..." && return 1
  
  local result=0
  
  # hours * 60 * 60
  result=$(expr "$hour" "*" "60")
  result=$(expr "$result" "*" "60")
  
  # (minutes * 60) + (hours * 60 * 60)
  minutes=$(expr "$minutes" "*" "60")
  result=$(expr "$result" "+" "$minutes")
  
  # seconds
  result=$(expr "$result" "+" "$seconds")
  
  printf "$result"
   
  return 0
}
######################################################################################

######################################################################################
# name        : get_difference_between_two_dates
# description : Obtain the difference between two dates in days. The dates must be in
#	              format yyyymmdd.
# parameters  : $1 first date.
#				        $2 second date.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_rigth_n_chars
# description : Obtain the n quantity of chars from a string begining at right.
# parameters  : $1 the string to parse.
#               $2 the n value
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_inode_name
# description : Obtain the inode name from a complete filename with path included 
#               excluding the extension of it.
# parameters  : $1 the inode to parse.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_inode_extension
# description : Obtain only the inode extension from a complete filename with path 
#               included.
# parameters  : $1 the inode to parse.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_time_from_last_inode_modification
# description : Obtain the time of the inode from the last modification in minutes. 
# parameters  : $1 the inode to verify.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_the_pid_of_daemon
# description : Obtain the pid of the program or shell script that match with the param.
# parameters  : $1 the string to find in the list of process.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : _quit_
# description : Finish the script when a signal is received. 
# parameters  : none.
# exit        : exit with 2 value.
#######################################################################################
_quit_()
{
  test -n "$!" && kill -0 "$!" 2>/dev/null && kill "$!" 2>/dev/null

  _exit 3 "Interrupt signal received, quiting ..."
}
#######################################################################################

#######################################################################################
# name        : _exit
# description : Exit from the current execution, login into the logfile and erase the flag-file.
# parameters  : $1 the exit code.
#               $2 the label to log in case of error.
# note        : Is needed to set the following config values before to call this function:
#                   
#               C_FLAGFILE_NAME
#
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
_exit()
{
  local exitCode=$( (test -n "$1" && printf "$1") || (printf $C_NOK) )
  local label=""
  
  if test -n "$2"
  then
    label="$2"
  else
    label="Termination with generic error"
  fi

  if test -z "$C_FLAGFILE_NAME"
  then
    C_FLAGFILE_NAME="$0"
  fi  
  
  if test $exitCode -eq $C_OK
  then
    log "The execution of [$0] was succefully."
  else
    log "The execution of [$0] was NOT OK."
    log "Error: [$label]."
  fi

  log "-------------------------------------------------------------------------------"
  
  erase_flagfile_for_instance_running
  
  exit $exitCode
}
#######################################################################################

#######################################################################################
# name        : check_if_there_is_another_instance_running
# description : Verify if there is another instance of the shell script running before.
#               If not exist another flag-file with the same name, this function create 
#               the flag-file and return OK as result.
# parameters  : none.
#
# note        : Is needed to set the following config values before to call this function:
#                   
#               C_FLAGFILE_NAME
#
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
check_if_there_is_another_instance_running()
{
  local rc=0
  local minutes_boundary_to_erase_flagfile=$( (test -n "$1" && printf "$1") || (printf "5") )
  local time_last_modif=0
  local pid_instance=0
  local flag_file=""
  local basename=""
  local path_tmp="/tmp/"
  
  test -z "$C_FLAGFILE_NAME" && log "check_if_there_is_another_instance_running(): Is nedeed to set C_FLAGFILE_NAME variable [$C_FLAGFILE_NAME]" && return $C_NOK

  local inode="$C_FLAGFILE_NAME" 

  basename=$(basename $inode); rc=$(expr $rc + $?)
  flag_file="$path_tmp""$basename"".flagfile"; rc=$(expr $rc + $?)
  
  # If not exist another flag-file with the same name this function create the flag-file and
  # return OK as result.
  test $rc -eq $C_OK && ! test -f "$flag_file" && echo "$$" > "$flag_file" && return $C_OK
  
  if test -f "$flag_file"
  then
    time_last_modif=$(get_time_from_last_inode_modification "$flag_file")

    if test $time_last_modif -gt $minutes_boundary_to_erase_flagfile
    then
      pid_instance=$(cat "$flag_file" | tr -d '\n' | tr -d '\r' 2>/dev/null)

      log "[my_pid=$$, my_parent_pid=$PPID]"
      log "[instance_running ==> [pid=$pid_instance]]"

      log "Killing [shell_script=$inode, pid=$pid_instance]."
      kill -15 "$pid_instance" 2>/dev/null

      log "Killing using brutal force of [shell_script=$inode, pid=$pid_instance]."
      kill -9 "$pid_instance" 2>/dev/null
      
      log "Deleting [flagfile=$flag_file], preventing an execution error, elapsed time [$time_last_modif] minutes..."
      rm -f "$flag_file" 2>/dev/null

      log "The flag-file exist and the time of the last update is greater than boundary time."
      log "That means that the previous script failed his execution."

      return $C_NOK
    else
      log "The flag-file exist and the time of the last update is less than boundary time."
    fi
  else
    log "The flag-file not exist but something is wrong if the function come to here."
    return $C_NOK
  fi

  log "There is another instance of this script running. Exiting..."
  log "script: [$basename]"
  log "-------------------------------------------------------------------------------"

  exit $C_NOK
}
#######################################################################################

#######################################################################################
# name        : erase_flagfile_for_instance_running
# description : Eliminate the flag file that represent the instance of the shell script running.
# parameters  : none.
#
# note        : Is needed to set the following config values before to call this function:
#                   
#               C_FLAGFILE_NAME
#
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
erase_flagfile_for_instance_running()
{
  local rc=0
  local path_tmp="/tmp/"
  
  test -z "$C_FLAGFILE_NAME" && log "erase_flagfile_for_instance_running(): Is nedeed to set C_FLAGFILE_NAME variable [$C_FLAGFILE_NAME]" && return $C_NOK

  local inode="$C_FLAGFILE_NAME"  
  
  local basename=$(basename $inode); rc=$(expr $rc + $?)
  
  local flag_file="$path_tmp""$basename"".flagfile"; rc=$(expr $rc + $?)
  
  test $rc -eq $C_OK && test -f "$flag_file" && rm -f "$flag_file" && return $C_OK

  return $C_NOK
}
#######################################################################################

#######################################################################################
# name        : touch_flagfile_for_instance_running
# description	: Update the time of the last modification of the flagfile.
# parameters  : none.
#
# note        : Is needed to set the following config values before to call this function:
#                   
#               C_FLAGFILE_NAME
#
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
touch_flagfile_for_instance_running()
{
  local rc=0
  local path_tmp="/tmp/"
  local current_time=$(date "+%s")

  test "$last_update_of_flagfile" -eq 0 && last_update_of_flagfile=$current_time
  
  local diff_between_times=$(($current_time-$last_update_of_flagfile))
  
  # if difference between current time and the last update of the flagfile is less
  # than 10 seconds we cancel the touch of the falgfile for performance reason.
  test "$diff_between_times" -le 10 && return $C_OK

  test -z "$C_FLAGFILE_NAME" && log "touch_flagfile_for_instance_running(): Is nedeed to set C_FLAGFILE_NAME variable [$C_FLAGFILE_NAME]" && return $C_NOK

  local inode="$C_FLAGFILE_NAME"
  
  local basename=$(basename $inode); rc=$(expr $rc + $?)
  
  local flag_file="$path_tmp""$basename"".flagfile"; rc=$(expr $rc + $?)

  test $rc -eq $C_OK && test -f "$flag_file" && touch "$flag_file" && last_update_of_flagfile=$current_time && return $C_OK

  return $C_NOK
}
#######################################################################################

#######################################################################################
# name        : sizeof
# description : return the size in bytes of the file.
# parameters  : $1 the file.
# return      : 0 if OK and print the size of the file, otherwise 1.
# example of use:
#         rc=$(sizeof ./how_to_build.txt)
#         $?  ==> 0    this value is produce by return call.
#         $rc ==> 5393 this is the size of the file in bytes.
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
# name        : match
# description : find a pattern in string, if it found then this function returns the 
#               index in the string where the pattern was found, it is between 1 to n.
#               Case no sensitive.
# parameters  : $1 the string.
#               $2 the pattern to find.
# return      : 0 if the pattern was found in the string and print the index, otherwise 1.
# example of use: 
#         rc=$(match "Where can i find a dog to buy?." "dog")
#         $?  ==> 0  this value is produce by return call.
#         $rc ==> 20 this is the index in the string 1-n
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
# name        : length
# description : Get the length of the string passed by parameter.
# parameters  : $1 the string.
# return      : 0 if OK and print the len of the string, otherwise 1.
# example of use: 
#         rc$(length "Where ")
#         $?  ==> 0  this value is produce by return call.
#         $rc ==> 6  this is the len of the string.
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
# name        : start_progress_indicator
# description : Create a new process to show a progress indication at console.
# parameters  : $1 time limit.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : stop_progress_indicator
# description : Stop the background process that show a progress indication at console.
# parameters  : none.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : get_current_shell
# description : Obtain the current shell that this shell script is running.
# parameters  : none.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : ask_for_y_or_n
# description : Ask for a question and wait a configurable time for the response at 
#               console from the user.
# parameters  : $1 the message that show for the question.
#               $2 the configurable time to wait for a response in seconds. The default
#               time to wait will be set to 45 seconds.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : ask_and_execute
# description : Ask and wait a configurable time for the response at console from the   
#               user and then will execute the command passed in the second parameter.  
#               This function is optimal to execute commands with parameters.
# parameters  : $1 the message that show for the question.
#               $2 the command to execute
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : ask_and_execute_function
# description : Ask and wait a configurable time for the response at console from the   
#               user and then will execute the function passed in the second parameter.  
#               This function is optimal to execute functions with parameters.
# parameters  : $1 the message that show for the question.
#               $2 the command to execute
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : warning
# description : Show a message warning at console in yellow.   
# parameters  : $1 the message to show at console.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : error
# description : Show a message error at console in red.   
# parameters  : $1 the message to show at console.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : success
# description : Show a message ok at console in green.   
# parameters  : $1 the message to show at console.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : fileCopy
# description : copy a file in a secure form (with retries), this function verify if 
#               the copy was ok and if there was an error retry the copy until to 
#               complete the total of retries. This function is useful when is needed 
#               to copy a file to network filesystem.
# parameters  : $1 file to copy.
#               $2 directory where the file will be put.
#               $3 retries.
# return      : C_OK if OK, otherwise C_NOK.
#
# example     :
#               fileCopy $PATH_SRC/file_test $PATH_TARGET
#               $?  ==> 0 this value is produce by return call.
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
# name        : check_directory
# description : Check if the directory passed by parameter exist and is writeable by
#               the user that run this script.   
# parameters  : $1 the dir to verify.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : check_or_create_directory
# description : Check if the directory passed by parameter exist and is writeable by
#               the user that run this script or try to create it.   
# parameters  : $1 the dir to verify.
# return      : C_OK if OK, otherwise C_NOK.
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
# name        : run_in_background
# description : Run the actual script in background.
# parameters  : none.
# return      : C_OK if OK, otherwise C_NOK.
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

#######################################################################################
# name        : verify_result_of_action
# description : Execute an action that is pass by parameter and the result will be check  
#               with another value pass by parameter too. Also check if that execution 
#               not complain with the expect result in certain quantity of times. 
# parameters  : $1 action to execute
#               $2 expected value, this value could be a list of values.
# return      : 0 if OK otherwise 1.
# example of use: 
#         verify_result_of_action check_status "opened|undefined"
#         $?  ==> 0  this value is produce by return call.
#######################################################################################
verify_result_of_action()
{
  local action="$1"
  local expected_values="$2"
  
  test -z "$action" && return 1
  test -z "$expected_values" && return 2
  
  local output=$($action 2>/dev/null)
  
  match "$expected_values" "$output" 1>/dev/null 2>&1
  
  test $? -eq 0 && return $C_OK
  
  return $C_NOK
}
#######################################################################################

#######################################################################################
# name        : is_holiday_date
# description : Verify if the current day is a holiday date. 
# parameters  : none.
# return      : 0 if is a holiday date otherwise 1.
# note        : Is needed to set C_HOLIDAYS_DAYS variable before to call this function. 
# example     :
#               C_HOLIDAYS_DAYS="10/10/2016,28/11/2016,08/12/2016,09/12/2016,25/12/2016"
#               is_holiday_date
#               $? ==> 0 means that the current day is holiday.
#######################################################################################
is_holiday_date()
{
  if test -z "$C_HOLIDAYS_DAYS"
  then
    log "One of the following config variables was not set:"
    log "C_HOLIDAYS_DAYS"
    return 1
  fi
    
  local date=$(date "+%d/%m/%Y" 2>/dev/null)
  
  match "$C_HOLIDAYS_DAYS" "$date" 1>/dev/null 2>&1
  
  test $? -eq 0 && log "[date=$date] is a holiday date." && return 0
  
  log "[date=$date] is not a holiday date."
  
  return 1
}
#######################################################################################

#######################################################################################
# name        : is_laboral_date
# description : Verify if the current day is a laboral date. 
# parameters  : none.
# return      : 0 if is a laboral date otherwise 1.
# note        : Is needed to set C_BUSINESS_DAYS variable before to call this function.
# example     :
#               C_BUSINESS_DAYS="lun|mar|mie|jue|vie|Mon|Tue|Wed|Thu|Fri"
#               is_laboral_date
#               $? ==> 0 means that the current date is a business day.
#######################################################################################
is_laboral_date()
{
  if test -z "$C_BUSINESS_DAYS"
  then
    log "One of the following config variables was not set:"
    log "C_BUSINESS_DAYS"
    return 1
  fi
    
  local name_day=$(date +%a 2>/dev/null)
  
  is_holiday_date
  test $? -ne 0 || return 1 

  match "$C_BUSINESS_DAYS" "$name_day" 1>/dev/null 2>&1
  test $? -eq 0 || return 1 
  
  log "[name_day=$name_day] is a business day."
  
  return 0
}
#######################################################################################

#######################################################################################
# name        : send_mail
# description : Send a notification to a recipient list. 
# parameters  : $1: is a subject.
#               $2: is the body of the notification.
# note        : Is needed to set the following config values before to call this function:
#               C_NOTIFICATION_RECIPIENTS="marianovolker@gmail.com fabricioreitano@gmail.com
#               C_ACCOUNT_MAIL_FROM="MarianoVolker@yahoo.com.ar"
#               C_ACCOUNT_USER_FROM="MarianoVolker"
#               C_ACCOUNT_PASSWD_FROM="xxdf"
#               C_SMTP_SERVER="smtp.mail.yahoo.com"
#
# return        : 0 for OK otherwise 1.
#######################################################################################
send_mail()
{
  if test -z "$C_ACCOUNT_MAIL_FROM" -o -z "$C_ACCOUNT_USER_FROM" -o -z "$C_ACCOUNT_PASSWD_FROM" -o -z "$C_SMTP_SERVER" -o -z "$C_NOTIFICATION_RECIPIENTS"
  then
    log "One of the following config variables was not set:"
    log "C_ACCOUNT_MAIL_FROM or C_ACCOUNT_USER_FROM or C_ACCOUNT_PASSWD_FROM or C_SMTP_SERVER or C_NOTIFICATION_RECIPIENTS"
    return 1
  fi
  
  local subject="$1"
  local body="$2"
  
  if test -z "$subject" -o -z "$body"
  then
    log "One of the following parameters was not set:"
    log "subject or body."    
    return 2
  fi
  
  local sendemail=$(type sendemail | tr -d '\n' | tr -d '\r' | cut -d" " -f3)
  
  if test -z "$sendemail"
  then
    log "The sendemail binary is not installed."
    return 3
  fi  
  
    
  local from="$C_ACCOUNT_MAIL_FROM"
  local user_from="$C_ACCOUNT_USER_FROM"
  local passwd_from="$C_ACCOUNT_PASSWD_FROM"
  local smtp_server="$C_SMTP_SERVER"
  local notification_recipients="$C_NOTIFICATION_RECIPIENTS"
  
  local err=0
  local output=""
  local output_total=""
  local mail=""
  
  #log "in with IFS=[$IFS]"
  
  local previous_send_mail_ifs=""
  previous_send_mail_ifs=$IFS
  
  IFS=' '
  
  #log "modifed to IFS=[$IFS]"
  
  for mail in $notification_recipients
  do
    log "[mail: $mail]"
    log "sendemail -f $from -u $subject -s $smtp_server -xu $user_from -xp $passwd_from -t $mail -m $body"
    output=$(sendemail -f $from -u "$subject" -s $smtp_server -xu $user_from -xp $passwd_from -t "$mail" -m "$body" 2>&1)
    err=$(expr $err + $?)
    output_total="$output_total""."$output    
  done
  IFS=$previous_send_mail_ifs
  
  #log "restoring to IFS=[$IFS]"
  
  test $err -gt 0 && log "" && log "ERROR: send_mail():[$output_total]" && log ""
  
  return $err
}
#######################################################################################

#######################################################################################
# name        : verify_retries_for_action
# description : Verify the retries for an action, if the max retries is exceded return 1. 
# parameters  : $1: action to verify as name of file.
#               $2: max retries.
#               $3: erase flag file
# return      : 0 if the retries is exceded, otherwise 1.
#######################################################################################
verify_retries_for_action()
{
  local file="$1"
  local max_retries="$2"
  local erase_flag_file="$3"
  
  if test -z "$file" -o -z "$max_retries"
  then
    log "One of the following parameters was not set:"
    log "action flag file or max_retries."    
    return 2
  fi
  
  file=$(echo -n "/tmp/""$file"".contador.rec")
  
  if test -n "$erase_flag_file" -o "$max_retries" -eq 0
  then
    test -f $file && rm -f $file
    return 1
  fi
  
  local MAX_QUANTITY_OF_RETRIES=$max_retries
  local retries=0
  local rc=1
  
  if test -f $file
  then
    retries=$(awk -F '=' '{printf "%d", $2}' $file 2>/dev/null)
    local real_retries=$(expr $retries + 1)
      
    if test "$real_retries" -ge "$MAX_QUANTITY_OF_RETRIES"
    then
      log "Retries exceded for action [$file]."
      rc=0
    else
      log "Retries was not exceded yet for action [$file]."
      rc=1
    fi
  fi

  retries=$(expr $retries + 1)
  echo "retries=$retries" > $file
  return $rc
}
#######################################################################################

#######################################################################################
# name        : verify_config_values
# description : Verify that all config values was set. 
# parameters  : $1: list of config values.
# return      : 0 if all values config was set, otherwise 1.
#######################################################################################
verify_config_values()
{
  local list_of_config_keys="$1"
 
  if test -z "$list_of_config_keys"
  then
    log "One of the following parameters was not set:"
    log "list_of_config_keys."
    return 2
  fi
  
  local rc=0
  local err=0
  local config_value=""
  local previous_verify_config_values_ifs=$IFS
  IFS=' '
  for config_key in $list_of_config_keys
  do
    eval config_value="\$$config_key"
    test -n "$config_value"
    err="$?"
    test "$err" -ne 0 && log "[config_key=$config_key]=[config_value=$config_value]" && log "$config_key was not set."
    rc=$(expr $rc + $err)
  done
  IFS=$previous_verify_config_values_ifs
  
  return $rc
}
#######################################################################################

#######################################################################################
# name        : get_current_path
# description : This function fix current path of this execution, that means the  
#               form like the shell script was call, if this is like "./script.sh" then 
#               the ./ is completed with the full path where the shell script is located.
# parameters  : none.
# return      : C_OK if OK, otherwise C_NOK.
#######################################################################################
get_current_path()
{
  local err=0
  local check=""
  local current_path=$(dirname $0)

  if test -z "$current_path"
  then
    current_path="./"
  else
    check=$(get_rigth_n_chars "$current_path" "1")
    test "$check" != "/" && current_path="$current_path""/"
  fi

  err=$(expr $err + $?)

  test $err -eq 0 && printf "$current_path" && return $C_OK

  return $C_NOK
}
#######################################################################################

#######################################################################################
# name        : execute_periodic_actions_alarms
# description : Verify each alarm/action if is periodic and is enable, in that case execute it.
# parameters  : $1: path to find actions or alarms
#               $2: type of actions or alarms.
# return      : 0 if OK, otherwise distinct of 0.
#######################################################################################
execute_periodic_actions_alarms()
{
  local path="$1"
  local type="$2"
  local config_dir=""
  local script_dir=""
  local script_to_execute=""
  local inode=""
  local err=0
  local periodic=0
  local enable=0
  local name_of_action_alarm=""
  
  test "$path" = "" && log "bad parameter 1" && return 1
  test "$type" != "ACTION" -a "$type" != "ALARM" && log "bad parameter 2" && return 1
  
  for inode in $(find "$path/" -name "*.conf" -type f)
  do
    periodic=0
    
    name_of_action_alarm=$(grep "C_NAME_OF_$type *" "$inode" | awk -F"=" '{printf "%s", $2}' | sed 's/"//g' 2>/dev/null)
    
    grep -q "C_PERIODIC_$type=1" "$inode"; periodic=$(expr $periodic + $?)
    

    if test "$periodic" -eq 0
    then
      config_dir=$(dirname "$inode")

      enable=0
      grep -q "C_ENABLE_$type=1" "$inode"; enable=$(expr $enable + $?)
      
      if test "$enable" -eq 0
      then

        log "[inode=$inode]"
        log "This alarm/action [$name_of_action_alarm] is enabled so now we will to execute it."

        script_dir="$config_dir""/../scripts"
        script_to_execute=$(ls $script_dir/*.sh 2>/dev/null)
        
        if test "$?" -eq 0 -a -r "$script_to_execute"
        then
          log "executing [nohup env -i sh -c \"$script_to_execute auto\" </dev/null >/dev/null 2>&1 &] ..."
          nohup env -i sh -c "$script_to_execute auto" </dev/null >/dev/null 2>&1 &
          err=$(expr $err + $?)
          my_sleep 0.5
        fi
      else
        
        log "[inode=$inode]"
        log "This alarm/action [$name_of_action_alarm] is disabled by configuration. So we need to put this status into the status file of the action/alarm."

        status_dir="$config_dir""/../status"
        status_file=$(ls $status_dir/*.tmp 2>/dev/null)
        
        if test "$?" -eq 0 -a -r "$status_file"
        then
          echo -n "disabled" > "$status_file"
        fi
      fi
    fi
    
  done
  
  return $err
}
#######################################################################################

#######################################################################################
# name        : is_the_time
# description : Verify if the current time is approximately equal to the specified one. 
# parameters  : none.
# return      : 0 if is the time otherwise 1.
# note        : Is needed to set C_RUN_AT_THE_TIME, C_RUN_AT_THE_TIME_DELTA_SECONDS variables 
#               before to call this function.
# example     :
#               C_RUN_AT_THE_TIME
#               C_RUN_AT_THE_TIME_DELTA_SECONDS
#               is_the_time
#               $? ==> 0 means that the current time is approximately equal to the specified one.
#
# return      : 0 for OK otherwise 1.
#######################################################################################
is_the_time()
{
  local err=0
  local config_values=""
  
  config_values="$config_values C_RUN_AT_THE_TIME "
  config_values="$config_values C_RUN_AT_THE_TIME_DELTA_SECONDS "
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi  

  local run_at_the_time_in_seconds=$(get_seconds_from_time "$C_RUN_AT_THE_TIME");err=$(expr $err + $?)
  local current_time=$(date +%H:%M:%S 2>/dev/null);err=$(expr $err + $?)
  local current_time_in_seconds=$(get_seconds_from_time "$current_time");err=$(expr $err + $?)
  
  log "[current_time=$current_time], [run_at_the_time=$C_RUN_AT_THE_TIME]."
  log "[current_time_in_seconds=$current_time_in_seconds], [run_at_the_time_in_seconds=$run_at_the_time_in_seconds]."
  
  test "$err" -ne 0 && log "One specificated date is not valid ..." && return 1
  
  local difference_in_seconds=$(expr "$current_time_in_seconds" - "$run_at_the_time_in_seconds")
  
  test "$difference_in_seconds" -lt 0 && difference_in_seconds=$(expr "$difference_in_seconds" "*" -1)
  
  if test "$difference_in_seconds" -le "$C_RUN_AT_THE_TIME_DELTA_SECONDS"
  then
    return 0
  fi  
  
  return 1
}
#######################################################################################

#######################################################################################
# name        : my_sleep
# description : Does a sleep interruptible by a system signal
# parameters  : $1 delay.
# return      : 0 if OK, otherwise 1.
#######################################################################################
my_sleep()
{
  local delay="$1"
  
  sleep "$delay" &
  wait

  return 0
}
#######################################################################################
