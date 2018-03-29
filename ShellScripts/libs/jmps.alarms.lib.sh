#!/bin/sh


#######################################################################################
# Depends of jmps.lib.sh
#######################################################################################

#######################################################################################
# Constants needed by this lib.
#######################################################################################
C_ALARMS_VERSION="1.1.0.1"
#######################################################################################

#######################################################################################
# name          : quit_alarm
# description   : Finish the alarm when a signal is received. 
# parameters    : none.
# exit          : exit with 2 value.
#######################################################################################
quit_alarm()
{
  _exit 2 "Interrupt signal received, quiting ..."
}
#######################################################################################

#######################################################################################
# name          : check_status_port
# description   : Obtain the status of the device and store this into a file. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ALARM
#                 C_PARAMETER_PORT
#                 C_STATUS_FILE
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#
# return        : 0 for OK otherwise 1.
#######################################################################################
check_status_port()
{
  local err=0
  local config_values=""
  
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ALARM "
  config_values="$config_values C_PARAMETER_PORT "
  config_values="$config_values C_STATUS_FILE "
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi  

  log "cd $C_PATH_BIN && $C_BINARY_NAME $C_PARAMETER_ALARM $C_PARAMETER_PORT"
  local output=$(cd $C_PATH_BIN && $C_BINARY_NAME $C_PARAMETER_ALARM $C_PARAMETER_PORT 2>&1)
  err=$(expr $err + $?)
  
  match "$output" "undefined"
  if test $? -eq 0
  then
    log "Status of alarm [undefined]."
    output=$(cd $C_PATH_BIN && $C_BINARY_NAME $C_PARAMETER_ALARM $C_PARAMETER_PORT 2>&1)
    err=$(expr $err + $?)
  fi
    
  log "Execute the alarm [$C_NAME_OF_ALARM]."
  log "This alarm does [$C_DESCRIPTION_OF_ALARM]."
  
  log "$output"
  
  echo -n $output
  echo -n $output > "$C_STATUS_FILE"
  touch "$C_STATUS_FILE"  

  return $err
}
#######################################################################################

#######################################################################################
# name          : should_be_open
# description   : Verify if is a laboral date and the current time is at the begining of
#                 a workday and the end of a workday. 
# parameters    : none.
# return        : 0 if this device should be open otherwise 1.
# note          : Is needed to set C_START_TIME_OF_WORKDAY, C_END_TIME_OF_WORKDAY variables before to call this function.
# example       :
#                 C_START_TIME_OF_WORKDAY
#                 C_END_TIME_OF_WORKDAY
#                 should_be_open
#                 $? ==> 0 means that the device should be open at current time of the day.
#
# return        : 0 for OK otherwise 1.
#######################################################################################
should_be_open()
{
  is_laboral_date
  test $? -eq 0 || return 1
  
  local err=0
  
  local workday_begin_in_seconds=$(get_seconds_from_time "$C_START_TIME_OF_WORKDAY");err=$(expr $err + $?)
  local workday_end_in_seconds=$(get_seconds_from_time "$C_END_TIME_OF_WORKDAY");err=$(expr $err + $?)
  local current_time=$(date +%H:%M:%S 2>/dev/null);err=$(expr $err + $?)
  local current_time_in_seconds=$(get_seconds_from_time "$current_time");err=$(expr $err + $?)
  
  log "[current_time=$current_time], [laboral_hour_begin=$C_START_TIME_OF_WORKDAY], [laboral_hour_end=$C_END_TIME_OF_WORKDAY]."
  log "[current_time_in_seconds=$current_time_in_seconds], [workday_begin_in_seconds=$workday_begin_in_seconds], [workday_end_in_seconds=$workday_end_in_seconds]."
  
  test "$err" -ne 0 && log "One specificated date is not valid ..." && return 1
  
  if test "$current_time_in_seconds" -ge "$workday_begin_in_seconds" -a "$current_time_in_seconds" -le "$workday_end_in_seconds"
  then
    log "[current_time=$current_time] at this time this device should be open by configuration."
    return 0
  fi  
  
  return 1
}
#######################################################################################

#######################################################################################
# name          : verify_last_execution_time
# description   : Verify the last execution time, if this value is less than configured
#                 one then this function return 1, does means that is not needed to run 
#                 so quickly as configured. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_RUN_EVERY_X_SECONDS
#                 C_PERIODIC_ALARM
#                 C_LAST_EXEC_FILE
#
# return        : 0 for OK otherwise 1.
#######################################################################################
verify_last_execution_time()
{
  if test -z "$C_RUN_EVERY_X_SECONDS" -o -z "$C_PERIODIC_ALARM" -o -z "$C_LAST_EXEC_FILE"
  then
    log "One of the following config variables was not set:"
    log "C_RUN_EVERY_X_SECONDS"
    log "C_PERIODIC_ALARM"
    log "C_LAST_EXEC_FILE"
    return 1
  fi
  
  log "[C_PERIODIC_ALARM=$C_PERIODIC_ALARM]"
  
  # If this alarm is not as periodic one, this could be run allways.
  test "$C_PERIODIC_ALARM" -ne 1 && return 0
    
  local last_modification_time_of_exec_file=$(stat --printf="%Y" "$C_LAST_EXEC_FILE" 2>/dev/null)
  
  ! test -f "$C_LAST_EXEC_FILE" && last_modification_time_of_exec_file=0
  
  log "[last_modification_time_of_exec_file=$last_modification_time_of_exec_file]"
  
  local timestamp=$(date +%s)
  
  local elapsed_time_from_last_execution=$(expr "$timestamp" - "$last_modification_time_of_exec_file")
  
  log "[elapsed_time_from_last_execution=$elapsed_time_from_last_execution]"
  
  test $elapsed_time_from_last_execution -lt "$C_RUN_EVERY_X_SECONDS" && log "too soon to execute this periodic alarm" && return 1

  touch "$C_LAST_EXEC_FILE"
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : verify_last_notification_time
# description   : Verify the last notification time, if this value is less than configured
#                 one then this function return 1. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_NOTIFY_EVERY_X_SECONDS
#                 C_PERIODIC_ALARM
#                 C_LAST_NOTIFICATION_FILE
#
# return        : 0 for OK otherwise 1.
#######################################################################################
verify_last_notification_time()
{
  if test -z "$C_NOTIFY_EVERY_X_SECONDS" -o -z "$C_PERIODIC_ALARM" -o -z "$C_LAST_NOTIFICATION_FILE"
  then
    log "One of the following config variables was not set:"
    log "C_NOTIFY_EVERY_X_SECONDS"
    log "C_PERIODIC_ALARM"
    log "C_LAST_NOTIFICATION_FILE"
    return 1
  fi
  
  log "[C_PERIODIC_ALARM=$C_PERIODIC_ALARM]"
  
  # If this alarm is not as periodic one, this could be run allways.
  test "$C_PERIODIC_ALARM" -ne 1 && return 0
    
  local last_modification_time_of_notification_file=$(stat --printf="%Y" "$C_LAST_NOTIFICATION_FILE" 2>/dev/null)
  
  ! test -f "$C_LAST_NOTIFICATION_FILE" && last_modification_time_of_notification_file=0
  
  log "[last_modification_time_of_notification_file=$last_modification_time_of_notification_file]"
  
  local timestamp=$(date +%s)
  
  local elapsed_time_from_last_notification=$(expr "$timestamp" - "$last_modification_time_of_notification_file")
  
  log "[elapsed_time_from_last_notification=$elapsed_time_from_last_notification]"
  
  test $elapsed_time_from_last_notification -lt "$C_NOTIFY_EVERY_X_SECONDS" &&  return 1

  touch "$C_LAST_NOTIFICATION_FILE"
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : periodic_local_check_of_alarm_with_notification
# description   : Verify the status of an action and emit a notification if correspond.
#                 Receive by parameter the status to verify and the action to execute.
#                 This function determines if this action should be executed periodically by 
#                 configuration.
# parameters    : $1: action to execute
#                 $2: status to verify.
#                 $3: type of device to verify.
#
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ALARM
#                 C_REQUIRE_NOTIFICATION
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ALARM
#                 C_PARAMETER_PORT
#                 C_RUN_EVERY_X_SECONDS
#
#                 The following values are for the notification purpouse ... 
#                 C_NOTIFY_EVERY_X_SECONDS
#                 C_HOLIDAYS_DAYS
#                 C_BUSINESS_DAYS
#                 C_START_TIME_OF_WORKDAY
#                 C_END_TIME_OF_WORKDAY
#                 C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED
#                 C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED
#                 C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED
#                 C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED
#                 C_NOTIFICATION_RECIPIENTS
#                 C_ACCOUNT_MAIL_FROM
#                 C_ACCOUNT_USER_FROM
#                 C_ACCOUNT_PASSWD_FROM
#                 C_SMTP_SERVER
#
# example of use: 
#                 periodic_local_check_of_alarm_with_notification "check_status_port" "opened|undefined" "puerta_controller_checker"
#                 $?  ==> 0  this value is produce by return call.
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
periodic_local_check_of_alarm_with_notification()
{
  local config_values=""
  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_REQUIRE_NOTIFICATION "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ALARM "
  config_values="$config_values C_PARAMETER_PORT "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  
  # for notification pourpouse ...
  config_values="$config_values C_NOTIFY_EVERY_X_SECONDS "
  config_values="$config_values C_HOLIDAYS_DAYS "
  config_values="$config_values C_BUSINESS_DAYS "
  config_values="$config_values C_START_TIME_OF_WORKDAY "
  config_values="$config_values C_END_TIME_OF_WORKDAY "
  config_values="$config_values C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED "
  config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED "
  config_values="$config_values C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED "
  config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED "
  config_values="$config_values C_NOTIFICATION_RECIPIENTS "
  config_values="$config_values C_ACCOUNT_MAIL_FROM "
  config_values="$config_values C_ACCOUNT_USER_FROM "
  config_values="$config_values C_ACCOUNT_PASSWD_FROM "
  config_values="$config_values C_SMTP_SERVER "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  local action_to_execute="$1"
  local status_to_verify="$2"
  local type_of_device="$3"
  
  if test -z "$action_to_execute" -o -z "$status_to_verify" -o -z "$type_of_device"
  then
    log "One of the following parameters was not set:"
    log "[action_to_execute=$action_to_execute], [status_to_verify=$status_to_verify], [type_of_device=$type_of_device]"    
    return 2
  fi

  verify_last_execution_time
  test $? -eq 0 || return 0

  local str_notification_body=""
  local str_notification_subject=""
  local time_stamp=$(date "+%d/%m/%Y_%H:%M:%S")
  
  should_be_open
  local should_be_open="$?"
  
  verify_result_of_action "$action_to_execute" "$status_to_verify"
  local status_port="$?"
 
  # Modify the environment variable to store the last notification time just for opened state ... 
  local C_LAST_NOTIFICATION_FILE_ORIGINAL="$C_LAST_NOTIFICATION_FILE"
  export C_LAST_NOTIFICATION_FILE="$C_LAST_NOTIFICATION_FILE_ORIGINAL""_opened"

  # Here we verify if the device is opened when this device must not be opened ...
  #-----------------------------------------------------------------------------
  if test $status_port -eq 0 -a $should_be_open -ne 0 -a "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED" -gt 0
  then
    log "$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED"
    
    verify_last_notification_time
    if test $? -eq 0
    then
      # Here we check if the retries were exceded ... 
      verify_retries_for_action "$type_of_device""_opened" "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED"
      if test $? -eq 0
      then
        # The retries were exceded so we set the msj to notify ...
        str_notification_subject="$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED" 
        str_notification_body="[$time_stamp] ==> [$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED]"
      fi
    fi
  else
    # If max retries is set to 0 this verification will not notify
    test "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED" -eq 0 && log "Notifications was disabled for alarm [$C_NAME_OF_ALARM] in case of openned state..."
    
    # Here we erase the retries file because the status of the device port is allowed ... 
    verify_retries_for_action "$type_of_device""_opened" "0" "erase"
    
    test -f "$C_LAST_NOTIFICATION_FILE" && rm -f "$C_LAST_NOTIFICATION_FILE"
    
    test $status_port -eq 0 -a $should_be_open -eq 0 && echo -n "allowed" > "$C_STATUS_FILE"
  fi
  #-----------------------------------------------------------------------------

  


  # Modify the environment variable to store the last notification time just for closed state ... 
  export C_LAST_NOTIFICATION_FILE="$C_LAST_NOTIFICATION_FILE_ORIGINAL""_closed"
  
  
  # Here we verify if the device is closed when this device must not be closed ...
  #-----------------------------------------------------------------------------
  if test $status_port -ne 0 -a $should_be_open -eq 0 -a "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED" -gt 0
  then
    log "$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED"

    verify_last_notification_time
    if test $? -eq 0
    then
	    # Here we check if the retries were exceded ...
	    verify_retries_for_action "$type_of_device""_closed" "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED"
	    if test $? -eq 0
	    then
	      # The retries were exceded so we set the msj to notify ...
	      str_notification_subject="$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED"
	      str_notification_body="[$time_stamp] ==> [$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED]"
	    fi
    fi
  else
    # If max retries is set to 0 this verification will not notify
    test "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED" -eq 0 && log "Notifications was disabled for alarm [$C_NAME_OF_ALARM] in case of closed state..."

    # Here we erase the retries file because the status of the device port is allowed ...
    verify_retries_for_action "$type_of_device""_closed" "0" "erase"
    
    test -f "$C_LAST_NOTIFICATION_FILE" && rm -f "$C_LAST_NOTIFICATION_FILE"
    
    test $status_port -ne 0 -a $should_be_open -ne 0 && echo -n "allowed" > "$C_STATUS_FILE"
  fi
  #-----------------------------------------------------------------------------

  test -n "$str_notification_subject" && log "Sending a notification." && send_mail "$str_notification_subject" "$str_notification_body"
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : periodic_local_check_of_alarm
# description   : Verify the status of an alarm.
#                 Receive by parameter the action to execute to verify the alarm.
#                 This function determines if this action should be executed periodically by 
#                 configuration.
# parameters    : $1: action to execute
#
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ALARM
#                 C_PERIODIC_ONLY
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ALARM
#                 C_PARAMETER_PORT
#                 C_RUN_EVERY_X_SECONDS
#                 C_STATUS_FILE
#                 C_LAST_EXEC_FILE
#
# example of use: 
#                 periodic_local_check_of_alarm "check_status_port"
#                 $?  ==> 0  this value is produce by return call.
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
periodic_local_check_of_alarm()
{
  local config_values=""
  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_PERIODIC_ONLY "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PATH_BIN "  
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ALARM "
  config_values="$config_values C_PARAMETER_PORT "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_STATUS_FILE "  
  config_values="$config_values C_LAST_EXEC_FILE "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  local action_to_execute="$1"
  
  if test -z "$action_to_execute"
  then
    log "One of the following parameters was not set:"
    log "[action_to_execute=$action_to_execute]"    
    return 2
  fi

  verify_last_execution_time
  test $? -eq 0 || return 0

  local output=$($action_to_execute 2>/dev/null)
  
  echo -n $output
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : periodic_local_check_of_alarm_at_the_time
# description   : Verify the status of an alarm.
#                 Receive by parameter the action to execute to verify the alarm.
#                 This function determines if this action should be executed periodically by 
#                 configuration.
# parameters    : $1: action to execute
#
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ALARM
#                 C_NEED_TO_RUN_AT_THE_TIME
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ALARM
#                 C_PARAMETER_PORT
#                 C_RUN_EVERY_X_SECONDS
#                 C_RUN_AT_THE_TIME
#                 C_RUN_AT_THE_TIME_DELTA_SECONDS
#
# example of use: 
#                 periodic_local_check_of_alarm_at_the_time "check_status_port"
#                 $?  ==> 0  this value is produce by return call.
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
periodic_local_check_of_alarm_at_the_time()
{
  local config_values=""
  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_NEED_TO_RUN_AT_THE_TIME "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ALARM "
  config_values="$config_values C_PARAMETER_PORT "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_RUN_AT_THE_TIME "
  config_values="$config_values C_RUN_AT_THE_TIME_DELTA_SECONDS "
  config_values="$config_values C_STATUS_FILE "  
  config_values="$config_values C_LAST_EXEC_FILE "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the action [$C_NAME_OF_ALARM]."
  log "This action does [$C_DESCRIPTION_OF_ALARM]."
  
  local action_to_execute="$1"
  
  if test -z "$action_to_execute"
  then
    log "One of the following parameters was not set:"
    log "[action_to_execute=$action_to_execute]"    
    return 2
  fi

  # Verified this value C_RUN_EVERY_X_SECONDS ...
  verify_last_execution_time
  test $? -eq 0 || return 0
  
  # Verified this value C_RUN_AT_THE_TIME and C_RUN_AT_THE_TIME_DELTA_SECONDS ...
  is_the_time
  test $? -eq 0 || return 0

  local output=$($action_to_execute 2>/dev/null)
  
  echo -n $output
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : execute_remote_alarm
# description   : Execute the remote alarm.
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_PARAMETER_ALARM_IP
#                 C_PARAMETER_ALARM_PORT
#                 C_PARAMETER_ALARM_USER
#                 C_PARAMETER_ALARM_TO_EXECUTE
#                 
# note          : To use this function is needed that interchange of SHA2 keys was be done 
#                 between both sites, local ip and remote ip.
#                 - http://www.thegeekstuff.com/2008/11/3-steps-to-perform-ssh-login-without-password-using-ssh-keygen-ssh-copy-id
#                 - https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_remote_alarm()
{
  local config_values=""
  
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PARAMETER_ALARM_IP "
  config_values="$config_values C_PARAMETER_ALARM_PORT "
  config_values="$config_values C_PARAMETER_ALARM_USER "
  config_values="$config_values C_PARAMETER_ALARM_TO_EXECUTE "  
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  local ip="$C_PARAMETER_ALARM_IP"
  local port="$C_PARAMETER_ALARM_PORT"
  local user="$C_PARAMETER_ALARM_USER"
  local alarm="$C_PARAMETER_ALARM_TO_EXECUTE"
  local rc=0
  
  if test -n "$ip" -a -n "$user" -a -n "$alarm"
  then
    
    log "$alarm"
    
    if test -n "$port"
    then 
      ssh -p "$port" "$user@$ip" "$alarm" 2>&1
      rc=$?
    else
      ssh "$user@$ip" "$alarm" 2>&1
      rc=$?
    fi
  
    return $rc
  else
    printf "The ip address, port address, user or alarm was not set."
  fi
  
  return 1
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_remote_alarm
# description   : Execute a periodic alarm. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_PERIODIC_ALARM
#                 C_PERIODIC_ONLY
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_RUN_EVERY_X_SECONDS
#                 C_STATUS_FILE
#                 C_PARAMETER_ALARM_IP
#                 C_PARAMETER_ALARM_PORT
#                 C_PARAMETER_ALARM_USER
#                 C_PARAMETER_ALARM_TO_EXECUTE  
#                 C_PARAMETER_ALARM_RETRIES
#                 C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES
#                 
# note 2        : To use this function is needed that interchange of SHA2 keys was be done 
#                 between both sites, local ip and remote ip.
#                 - http://www.thegeekstuff.com/2008/11/3-steps-to-perform-ssh-login-without-password-using-ssh-keygen-ssh-copy-id
#                 - https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server
#
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_remote_alarm()
{
  local err=0
  local output=""
  local try=0
  local config_values=""

  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_PERIODIC_ONLY "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ALARM_IP "
  config_values="$config_values C_PARAMETER_ALARM_PORT "
  config_values="$config_values C_PARAMETER_ALARM_USER "
  config_values="$config_values C_PARAMETER_ALARM_TO_EXECUTE "  
  config_values="$config_values C_PARAMETER_ALARM_RETRIES "
  config_values="$config_values C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES "  
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the alarm [$C_NAME_OF_ALARM]."
  log "This alarm does [$C_DESCRIPTION_OF_ALARM]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  while test "$try" -lt "$C_PARAMETER_ALARM_RETRIES"
  do
    output=$(execute_remote_alarm 2>&1)
    err=$(expr $err + $?)

    log "execute_remote_alarm [name:$C_NAME_OF_ALARM][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running
    
    my_sleep "$C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done

  touch "$C_STATUS_FILE"
  
  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_remote_alarm_at_the_time
# description   : Execute a periodic alarm at specific time. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ALARM
#                 C_NEED_TO_RUN_AT_THE_TIME
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_RUN_EVERY_X_SECONDS
#                 C_STATUS_FILE
#                 C_PARAMETER_ALARM_IP
#                 C_PARAMETER_ALARM_PORT
#                 C_PARAMETER_ALARM_USER
#                 C_PARAMETER_ALARM_TO_EXECUTE
#                 C_PARAMETER_ALARM_RETRIES
#                 C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_remote_alarm_at_the_time()
{
  local err=0
  local output=""
  local try=0
  local config_values=""

  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_NEED_TO_RUN_AT_THE_TIME "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ALARM_IP "
  config_values="$config_values C_PARAMETER_ALARM_PORT "
  config_values="$config_values C_PARAMETER_ALARM_USER "
  config_values="$config_values C_PARAMETER_ALARM_TO_EXECUTE "
  config_values="$config_values C_PARAMETER_ALARM_RETRIES "
  config_values="$config_values C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the alarm [$C_NAME_OF_ALARM]."
  log "This alarm does [$C_DESCRIPTION_OF_ALARM]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  # Verified this value C_RUN_AT_THE_TIME and C_RUN_AT_THE_TIME_DELTA_SECONDS ...
  is_the_time
  test $? -eq 0 || return 0
  
  while test "$try" -lt "$C_PARAMETER_ALARM_RETRIES"
  do
    output=$(execute_remote_alarm 2>&1)
    err=$(expr $err + $?)

    log "execute_remote_alarm[name:$C_NAME_OF_ALARM][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running

    my_sleep "$C_PARAMETER_ALARM_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done    

  echo -n $output

  touch "$C_STATUS_FILE"

  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_remote_alarm_with_notification
# description   : Verify the status of an action and emit a notification if correspond.
#                 Receive by parameter the status to verify and the action to execute.
#                 This function determines if this action should be executed periodically by 
#                 configuration.
# parameters    : $1: action to execute
#                 $2: status to verify.
#                 $3: type of device to verify.
#
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ALARM
#                 C_REQUIRE_NOTIFICATION
#                 C_NAME_OF_ALARM
#                 C_DESCRIPTION_OF_ALARM
#                 C_PARAMETER_ALARM_IP
#                 C_PARAMETER_ALARM_PORT
#                 C_PARAMETER_ALARM_USER
#                 C_PARAMETER_ALARM_TO_EXECUTE
#                 C_RUN_EVERY_X_SECONDS
#
#                 The following values are for the notification purpouse ... 
#                 C_NOTIFY_EVERY_X_SECONDS
#                 C_HOLIDAYS_DAYS
#                 C_BUSINESS_DAYS
#                 C_START_TIME_OF_WORKDAY
#                 C_END_TIME_OF_WORKDAY
#                 C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED
#                 C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED
#                 C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED
#                 C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED
#                 C_NOTIFICATION_RECIPIENTS
#                 C_ACCOUNT_MAIL_FROM
#                 C_ACCOUNT_USER_FROM
#                 C_ACCOUNT_PASSWD_FROM
#                 C_SMTP_SERVER
#
# example of use: 
#                 execute_periodic_remote_alarm_with_notification "execute_periodic_remote_alarm" "opened|undefined" "puerta_controller_checker"
#                 $?  ==> 0  this value is produce by return call.
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_remote_alarm_with_notification()
{
  local config_values=""
  config_values="$config_values C_PERIODIC_ALARM "
  config_values="$config_values C_REQUIRE_NOTIFICATION "
  config_values="$config_values C_NAME_OF_ALARM "
  config_values="$config_values C_DESCRIPTION_OF_ALARM "
  config_values="$config_values C_PARAMETER_ALARM_IP "
  config_values="$config_values C_PARAMETER_ALARM_PORT "
  config_values="$config_values C_PARAMETER_ALARM_USER "
  config_values="$config_values C_PARAMETER_ALARM_TO_EXECUTE "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  
  # for notification pourpouse ...
  config_values="$config_values C_NOTIFY_EVERY_X_SECONDS "
  config_values="$config_values C_HOLIDAYS_DAYS "
  config_values="$config_values C_BUSINESS_DAYS "
  config_values="$config_values C_START_TIME_OF_WORKDAY "
  config_values="$config_values C_END_TIME_OF_WORKDAY "
  config_values="$config_values C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED "
  config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED "
  config_values="$config_values C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED "
  config_values="$config_values C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED "
  config_values="$config_values C_NOTIFICATION_RECIPIENTS "
  config_values="$config_values C_ACCOUNT_MAIL_FROM "
  config_values="$config_values C_ACCOUNT_USER_FROM "
  config_values="$config_values C_ACCOUNT_PASSWD_FROM "
  config_values="$config_values C_SMTP_SERVER "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  local action_to_execute="$1"
  local status_to_verify="$2"
  local type_of_device="$3"
  
  if test -z "$action_to_execute" -o -z "$status_to_verify" -o -z "$type_of_device"
  then
    log "One of the following parameters was not set:"
    log "[action_to_execute=$action_to_execute], [status_to_verify=$status_to_verify], [type_of_device=$type_of_device]"    
    return 2
  fi

  verify_last_execution_time
  test $? -eq 0 || return 0

  local str_notification_body=""
  local str_notification_subject=""
  local time_stamp=$(date "+%d/%m/%Y_%H:%M:%S")
  
  should_be_open
  local should_be_open="$?"
  
  verify_result_of_action "$action_to_execute" "$status_to_verify"
  local status_port="$?"
 
  # Modify the environment variable to store the last notification time just for opened state ... 
  local C_LAST_NOTIFICATION_FILE_ORIGINAL="$C_LAST_NOTIFICATION_FILE"
  export C_LAST_NOTIFICATION_FILE="$C_LAST_NOTIFICATION_FILE_ORIGINAL""_opened"

  # Here we verify if the device is opened when this device must not be opened ...
  #-----------------------------------------------------------------------------
  if test $status_port -eq 0 -a $should_be_open -ne 0 -a "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED" -gt 0
  then
    log "$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED"
    
    verify_last_notification_time
    if test $? -eq 0
    then
      # Here we check if the retries were exceded ... 
      verify_retries_for_action "$type_of_device""_opened" "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED"
      if test $? -eq 0
      then
        # The retries were exceded so we set the msj to notify ...
        str_notification_subject="$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED" 
        str_notification_body="[$time_stamp] ==> [$C_MSJ_DEVICE_OPENED_WHEN_SHOULD_BE_CLOSED]"
      fi
    fi
  else
    # If max retries is set to 0 this verification will not notify
    test "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_OPENED_WHEN_SHOULD_BE_CLOSED" -eq 0 && log "Notifications was disabled for alarm [$C_NAME_OF_ALARM] in case of openned state..."
    
    # Here we erase the retries file because the status of the device port is allowed ... 
    verify_retries_for_action "$type_of_device""_opened" "0" "erase"
    
    test -f "$C_LAST_NOTIFICATION_FILE" && rm -f "$C_LAST_NOTIFICATION_FILE"
    
    test $status_port -eq 0 -a $should_be_open -eq 0 && echo -n "allowed" > "$C_STATUS_FILE"
  fi
  #-----------------------------------------------------------------------------

  


  # Modify the environment variable to store the last notification time just for closed state ... 
  export C_LAST_NOTIFICATION_FILE="$C_LAST_NOTIFICATION_FILE_ORIGINAL""_closed"
  
  
  # Here we verify if the device is closed when this device must not be closed ...
  #-----------------------------------------------------------------------------
  if test $status_port -ne 0 -a $should_be_open -eq 0 -a "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED" -gt 0
  then
    log "$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED"

    verify_last_notification_time
    if test $? -eq 0
    then
      # Here we check if the retries were exceded ...
      verify_retries_for_action "$type_of_device""_closed" "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED"
      if test $? -eq 0
      then
        # The retries were exceded so we set the msj to notify ...
        str_notification_subject="$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED"
        str_notification_body="[$time_stamp] ==> [$C_MSJ_DEVICE_CLOSED_WHEN_SHOULD_BE_OPENED]"
      fi
    fi
  else
    # If max retries is set to 0 this verification will not notify
    test "$C_MAX_RETRIES_BEFORE_TO_ANOUNCE_THAT_THE_DEVICE_IS_CLOSED_WHEN_SHOULD_BE_OPENED" -eq 0 && log "Notifications was disabled for alarm [$C_NAME_OF_ALARM] in case of closed state..."

    # Here we erase the retries file because the status of the device port is allowed ...
    verify_retries_for_action "$type_of_device""_closed" "0" "erase"
    
    test -f "$C_LAST_NOTIFICATION_FILE" && rm -f "$C_LAST_NOTIFICATION_FILE"
    
    test $status_port -ne 0 -a $should_be_open -ne 0 && echo -n "allowed" > "$C_STATUS_FILE"
  fi
  #-----------------------------------------------------------------------------

  test -n "$str_notification_subject" && log "Sending a notification." && send_mail "$str_notification_subject" "$str_notification_body"
  
  return 0
}
#######################################################################################

