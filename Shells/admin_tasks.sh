#!/bin/sh

#######################################################################################
# name          : _init_
# description   : Initialize the script and load all libraries. 
# parameters    : none.
# return        : 0 for OK otherwise 1.
#######################################################################################
_init_()
{
  local DIR=$(dirname $0)
  
  C_JMPS_BASE_DIR=$DIR"/../.."
  C_JMPS_BACKUP_CONFIG_DIR=$C_JMPS_BASE_DIR"/config_backups"
  C_JMPS_CONFIG_FILE=$DIR"/../../config/etc/jmps.conf"
  C_JMPS_LIB_FILE=$DIR"/../../libs/jmps.lib.sh"
  C_PATH_LOG=$DIR"/../../logs/etc/"
  
  test -r $C_JMPS_LIB_FILE && . $C_JMPS_LIB_FILE                 ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_LIB_FILE]" && exit 1
  test -r $C_JMPS_CONFIG_FILE && . $C_JMPS_CONFIG_FILE           ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_CONFIG_FILE]" && exit 1
  test -d $C_PATH_LOG                                            ; test $? -ne 0 && echo "ERROR: no es un directorio valido [$C_PATH_LOG]" && exit 1

  log "-------------------------------------------------------------------------------"
      
  test $C_DEBUG -eq 1 && set -x

  C_FLAGFILE_NAME="$0"

  # If there exist another instance running interrupt the execution of this script.
  check_if_there_is_another_instance_running
  
  # Trap any signal between 1-30 and call quit function to log and exit cleanly.
  trap _quit_ 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30  
}
#######################################################################################

#######################################################################################
# name          : obtain_system_information
# description   : Obtain system information.
# parameters    : none
# return        : 0 if OK, otherwise distinct of 0.
#######################################################################################
obtain_system_information()
{
  local err=0
  
  test -z "$C_PATH_FOR_VIDEO_DEVICES" && export C_PATH_FOR_VIDEO_DEVICES="/opt/jmps/streams/"
  test -z "$C_SYSTEM_STATUS_FILENAME" && export C_SYSTEM_STATUS_FILENAME="/opt/jmps/status/system.st"
  
  local system_time=$(date "+%Y/%m/%d_%H:%M:%S"); err=$(expr $err + $?)
  log "system_time; err=$err"
  
  local system_uptime=""
  system_uptime=$(uptime | tr -d '\n' | tr -d '\r' 2>/dev/null); err=$(expr $err + $?)
  log "system_uptime; err=$err"  
  
  local size_in_recorded_files=$(du -h -s -c "$C_PATH_FOR_VIDEO_DEVICES" | tail -1 | awk '{printf "%s", $1}' 2>/dev/null); err=$(expr $err + $?)
  log "size_in_recorded_files; err=$err"
  
  local disksize=$(df -h / | tail -1 | awk '{printf "%s", $2}' 2>/dev/null); err=$(expr $err + $?)
  log "disksize; err=$err"
  
  local available_disksize=$(df -h / | tail -1 | awk '{printf "%s", $4}' 2>/dev/null); err=$(expr $err + $?)
  log "available_disksize; err=$err"
  
  echo "JMPS_SYSTEM_TIME=$system_time" > "$C_SYSTEM_STATUS_FILENAME"; err=$(expr $err + $?)
  log "JMPS_SYSTEM_TIME; err=$err"
  
  echo "JMPS_SIZE_IN_RECORDED_FILES=$size_in_recorded_files" >> "$C_SYSTEM_STATUS_FILENAME"; err=$(expr $err + $?)
  log "JMPS_SIZE_IN_RECORDED_FILES; err=$err"
  
  echo "JMPS_SYSTEM_DISK_SIZE=$disksize" >> "$C_SYSTEM_STATUS_FILENAME"; err=$(expr $err + $?)
  log "JMPS_SYSTEM_DISK_SIZE; err=$err"
  
  echo "JMPS_SYSTEM_UPTIME=$system_uptime" >> "$C_SYSTEM_STATUS_FILENAME"; err=$(expr $err + $?)
  log "JMPS_SYSTEM_UPTIME; err=$err"  
  
  echo "JMPS_SYSTEM_AVAILABLE_DISK_SIZE=$available_disksize" >> "$C_SYSTEM_STATUS_FILENAME"; err=$(expr $err + $?)
  log "JMPS_SYSTEM_AVAILABLE_DISK_SIZE; err=$err"  

  local ups_output=$(/sbin/apcaccess | awk -F':' '{printf "UPS_%s=%s\n", $1, $2}' >> "$C_SYSTEM_STATUS_FILENAME" 2>/dev/null)
  log "ups_output; err=$err"

  # Verify if the UPS show us if the STATUS of the line is on Battery or on-line in that case we need to send an email ...
  # These constants are needed to be set on jmps.conf file.
  # C_ACCOUNT_MAIL_FROM
  # C_ACCOUNT_USER_FROM
  # C_ACCOUNT_PASSWD_FROM
  # C_SMTP_SERVER
  # C_NOTIFICATION_RECIPIENTS
  # C_MSJ_UPS_IS_ON_BATTERY_STATE
  if test "$C_MSJ_UPS_IS_ON_BATTERY_STATE" != ""
  then 
    local ups_status=$(grep -w "UPS_STATUS" "$C_SYSTEM_STATUS_FILENAME" | awk -F'=' '{printf "%s", $2}' | tr -d ' ' | tr -d '\n' | tr -d '\r' 2>/dev/null); err=$(expr $err + $?)
    log "ups_status; err=$err"
    if test $err -eq 0 -a "$ups_status" != "ONLINE"
    then
      local str_notification_subject="$C_MSJ_UPS_IS_ON_BATTERY_STATE"
      local str_notification_body="[$system_time] ==> [$C_MSJ_UPS_IS_ON_BATTERY_STATE][ESTADO=$ups_status]."      
      send_mail "$str_notification_subject" "$str_notification_body"
      err=$(expr $err + $?)
      log "send_mail; err=$err"
    fi
  fi

  return $err
}
#######################################################################################

#######################################################################################
# name          : main
# description   : Main function of the shell script.
# parameters    : none.
# return        : C_OK if OK, otherwise C_NOK.
#######################################################################################
main()
{
  #------------------------------------------------------------------------------------
  _init_
  #------------------------------------------------------------------------------------

  #------------------------------------------------------------------------------------
  local err=0
  local str=""
  
  test -z "$C_TIME_TO_RUN_ADMIN_TASKS" && export C_TIME_TO_RUN_ADMIN_TASKS=80
  
  while test "$err" -eq 0
  do

    log "obtain_system_information"
    obtain_system_information
    err=$(expr $err + $?)    
    
    touch_flagfile_for_instance_running

    my_sleep $C_TIME_TO_RUN_ADMIN_TASKS
  done
  #------------------------------------------------------------------------------------

  #------------------------------------------------------------------------------------
  _exit $err "$str"
  #------------------------------------------------------------------------------------
}
#######################################################################################

#######################################################################################
main
#######################################################################################

