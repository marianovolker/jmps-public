#!/bin/sh

#######################################################################################
# name          : init_alarm
# description   : Initialize the alarm and load all libraries. 
# parameters    : none.
# return        : 0 for OK otherwise 1.
#######################################################################################
init_alarm()
{
  local DIR=$(dirname $0)

  C_JMPS_CONFIG_FILE=$DIR"/../../../config/etc/jmps.conf"
  C_JMPS_LIB_FILE=$DIR"/../../../libs/jmps.lib.sh"
  C_JMPS_ALARMS_LIB_FILE=$DIR"/../../../libs/jmps.alarms.lib.sh"
  C_PATH_LOG=$DIR"/../logs/"
  C_PATH_BIN=$DIR"/../../../bin/eys/"
  C_CONFIG_FILE=$DIR"/../config/alarm.conf"
  C_STATUS_FILE=$DIR"/../status/status.tmp"
  C_LAST_EXEC_FILE=$DIR"/../status/last_execution_time.tmp"
  C_LAST_NOTIFICATION_FILE=$DIR"/../status/last_notification_time.tmp"
  C_BINARY_NAME="./eys_multipurpose_controller"
  C_PARAMETER_ALARM="status"

  test -r $C_JMPS_CONFIG_FILE && . $C_JMPS_CONFIG_FILE            ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_CONFIG_FILE]" && exit 1
  test -r $C_CONFIG_FILE && . $C_CONFIG_FILE                      ; test $? -ne 0 && echo "ERROR: loading [$C_CONFIG_FILE]" && exit 1  
  test -r $C_JMPS_LIB_FILE && . $C_JMPS_LIB_FILE                  ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_LIB_FILE]" && exit 1
  test -r $C_JMPS_ALARMS_LIB_FILE && . $C_JMPS_ALARMS_LIB_FILE    ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_ALARMS_LIB_FILE]" && exit 1
  test -x "$C_PATH_BIN""$C_BINARY_NAME"                           ; test $? -ne 0 && echo "ERROR: is not executable by current user ["$C_PATH_BIN""$C_BINARY_NAME"]" && exit 1
  test -d $C_PATH_LOG                                             ; test $? -ne 0 && echo "ERROR: no es un directorio valido [$C_PATH_LOG]" && exit 1
  test -d $C_PATH_BIN                                             ; test $? -ne 0 && echo "ERROR: no es un directorio valido [$C_PATH_BIN]" && exit 1

  log "-------------------------------------------------------------------------------"
      
  test $C_DEBUG -eq 1 && set -x

  local config_values=""

  config_values="$config_values C_ENABLE_ALARM "
  config_values="$config_values C_PARAMETER_ALARM "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  
  if test "$C_PERIODIC_ALARM" = "1"
  then
    if test "$C_PERIODIC_ONLY" = "1"
    then
      config_values="$config_values C_RUN_EVERY_X_SECONDS "
      
    elif test "$C_NEED_TO_RUN_AT_THE_TIME" = "1"
    then
      config_values="$config_values C_RUN_AT_THE_TIME "
      config_values="$config_values C_RUN_AT_THE_TIME_DELTA_SECONDS "
      config_values="$config_values C_RUN_EVERY_X_SECONDS "
      
    elif test "$C_REQUIRE_NOTIFICATION" = "1"
    then
      config_values="$config_values C_RUN_EVERY_X_SECONDS "
      config_values="$config_values C_NOTIFY_EVERY_X_SECONDS "
      config_values="$config_values C_HOLIDAYS_DAYS "
      config_values="$config_values C_BUSINESS_DAYS "
      config_values="$config_values C_START_TIME_OF_WORKDAY "
      config_values="$config_values C_END_TIME_OF_WORKDAY "
      config_values="$config_values C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED "
      config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED "
      config_values="$config_values C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED "
      config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED "      
    fi    
  fi  

  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    exit 1
  fi
  
  # If this alarm is not enabled then exit without an error condition ...
  test "$C_ENABLE_ALARM" != "1" && log "This alarm [$C_NAME_OF_ALARM] is disabled by configuration ..." && exit 0

  C_FLAGFILE_NAME="$0""_""$C_NAME_OF_ALARM"

  # If there exist another instance running interrupt the execution of this script.
  check_if_there_is_another_instance_running

  # Trap any signal between 1-30 and call quit function to log and exit cleanly.
  trap quit_alarm 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30  
}
#######################################################################################

#######################################################################################
main()
{
  #----------------------------------------------------------------------------------
  init_alarm
  #----------------------------------------------------------------------------------

  #----------------------------------------------------------------------------------
  local err=0
  local alarm="$1"
  local str=""
  local output=""

  case "$alarm" in
    "status")
      log "check_status_port"
      output=$(check_status_port)
      err=$(expr $err + $?)
      log "$output"
    ;;
    
    "auto")
      if test "$C_PERIODIC_ALARM" = "1"
      then
        if test "$C_PERIODIC_ONLY" = "1"
        then
          output=$(periodic_local_check_of_alarm "check_status_port")
          
        elif test "$C_NEED_TO_RUN_AT_THE_TIME" = "1"
        then
          output=$(periodic_local_check_of_alarm_at_the_time "check_status_port")
            
        elif test "$C_REQUIRE_NOTIFICATION" = "1"
        then
          output=$(periodic_local_check_of_alarm_with_notification "check_status_port" "opened|undefined" "$C_NAME_OF_ALARM")
        fi    
      fi      
      err=$(expr $err + $?)
      log "output=[$output]"
    ;;
    
    *)
      str="invalid parameter [$alarm]."
      err=1
    ;;
  esac 
  #----------------------------------------------------------------------------------

  #----------------------------------------------------------------------------------
  _exit $err "$str"
  #---------------------------------------------------------------------------------- 
}
#######################################################################################

#######################################################################################
main "$1"
#######################################################################################
