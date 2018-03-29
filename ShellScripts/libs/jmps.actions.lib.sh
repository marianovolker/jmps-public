#!/bin/sh


#######################################################################################
# Depends of jmps.lib.sh
#######################################################################################

#######################################################################################
# Constants needed by this lib.
#######################################################################################
C_ACTIONS_LIB_VERSION="1.1.0.1"
#######################################################################################

#######################################################################################
# name          : quit_action
# description   : Finish the action when a signal is received. 
# parameters    : none.
# exit          : exit with 2 value.
#######################################################################################
quit_action()
{
  _exit 2 "Interrupt signal received, quiting ..."
}
#######################################################################################

#######################################################################################
# name          : verify_last_execution_time
# description   : Verify the last execution time, if this value is less than configured
#                 one then this function return 1, does means that is to soon to execute it.
# 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_RUN_EVERY_X_SECONDS
#                 C_PERIODIC_ACTION
#                 C_STATUS_FILE
#
# return        : 0 for OK otherwise 1.
#######################################################################################
verify_last_execution_time()
{
  if test -z "$C_RUN_EVERY_X_SECONDS" -o -z "$C_PERIODIC_ACTION"
  then
    log "One of the following config variables was not set:"
    log "C_RUN_EVERY_X_SECONDS"
    log "C_PERIODIC_ACTION"
    return 1
  fi
  
  log "C_PERIODIC_ACTION=$C_PERIODIC_ACTION"
  
  # If this action is not a periodic one, this could be run allways.
  test "$C_PERIODIC_ACTION" -ne 1 && return 0

  local last_modification_time_of_status_file=$(stat --printf="%Y" "$C_STATUS_FILE" 2>/dev/null)
  
  ! test -f "$C_STATUS_FILE" && last_modification_time_of_status_file=0
  
  local timestamp=$(date +%s)
  
  local elapsed_time_from_last_execution=$(expr "$timestamp" - "$last_modification_time_of_status_file")
  
  local elapsed_time_from_last_execution_in_hours=$(expr "$elapsed_time_from_last_execution" / "3600")
  
  local elapsed_time_from_last_execution_in_mins=$(expr "$elapsed_time_from_last_execution" % "3600" / 60)
  
  log "elapsed time from last execution in hours [$elapsed_time_from_last_execution_in_hours hours and $elapsed_time_from_last_execution_in_mins mins]"
  
  log "C_RUN_EVERY_X_SECONDS=$C_RUN_EVERY_X_SECONDS"
  
  test $elapsed_time_from_last_execution -lt "$C_RUN_EVERY_X_SECONDS" && log "too soon to execute this periodic action" && return 1
  
  return 0
}
#######################################################################################

#######################################################################################
# name          : execute_remote_action
# description   : Execute the remote action.
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_PARAMETER_ACTION_IP
#                 C_PARAMETER_ACTION_PORT
#                 C_PARAMETER_ACTION_USER
#                 C_PARAMETER_ACTION_TO_EXECUTE
#                 
# note          : To use this function is needed that interchange of SHA2 keys was be done 
#                 between both sites, local ip and remote ip.
#                 - http://www.thegeekstuff.com/2008/11/3-steps-to-perform-ssh-login-without-password-using-ssh-keygen-ssh-copy-id
#                 - https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_remote_action()
{
  local config_values=""
  
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_PARAMETER_ACTION_IP "
  config_values="$config_values C_PARAMETER_ACTION_PORT "
  config_values="$config_values C_PARAMETER_ACTION_USER "
  config_values="$config_values C_PARAMETER_ACTION_TO_EXECUTE "  
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  local ip="$C_PARAMETER_ACTION_IP"
  local port="$C_PARAMETER_ACTION_PORT"
  local user="$C_PARAMETER_ACTION_USER"
  local action="$C_PARAMETER_ACTION_TO_EXECUTE"
  local rc=0
  
  if test -n "$ip" -a -n "$user" -a -n "$action"
  then
    
    log "$action"
    
    if test -n "$port"
    then 
      ssh -p "$port" "$user@$ip" "$action" 2>&1
      rc=$?
    else
      ssh "$user@$ip" "$action" 2>&1
      rc=$?
    fi
  
    return $rc
  else
    printf "The ip address, port address, user or action was not set."
  fi
  
  return 1
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_remote_action
# description   : Execute a periodic action. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_PERIODIC_ACTION
#                 C_PERIODIC_ONLY
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_RUN_EVERY_X_SECONDS
#                 C_STATUS_FILE
#                 C_PARAMETER_ACTION_IP
#                 C_PARAMETER_ACTION_PORT
#                 C_PARAMETER_ACTION_USER
#                 C_PARAMETER_ACTION_TO_EXECUTE  
#                 C_PARAMETER_ACTION_RETRIES
#                 C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES
#                 
# note 2        : To use this function is needed that interchange of SHA2 keys was be done 
#                 between both sites, local ip and remote ip.
#                 - http://www.thegeekstuff.com/2008/11/3-steps-to-perform-ssh-login-without-password-using-ssh-keygen-ssh-copy-id
#                 - https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server
#
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_remote_action()
{
  local err=0
  local output=""
  local try=0
  local config_values=""

  config_values="$config_values C_PERIODIC_ACTION "
  config_values="$config_values C_PERIODIC_ONLY "
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ACTION_IP "
  config_values="$config_values C_PARAMETER_ACTION_PORT "
  config_values="$config_values C_PARAMETER_ACTION_USER "
  config_values="$config_values C_PARAMETER_ACTION_TO_EXECUTE "  
  config_values="$config_values C_PARAMETER_ACTION_RETRIES "
  config_values="$config_values C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES "  
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the action [$C_NAME_OF_ACTION]."
  log "This action does [$C_DESCRIPTION_OF_ACTION]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  while test "$try" -lt "$C_PARAMETER_ACTION_RETRIES"
  do
    output=$(execute_remote_action 2>&1)
    err=$(expr $err + $?)

    log "execute_remote_action [name:$C_NAME_OF_ACTION][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running
    
    my_sleep "$C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done

  touch "$C_STATUS_FILE"
  
  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_remote_action_at_the_time
# description   : Execute a periodic action at specific time. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ACTION
#                 C_NEED_TO_RUN_AT_THE_TIME
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_RUN_EVERY_X_SECONDS
#                 C_STATUS_FILE
#                 C_PARAMETER_ACTION_IP
#                 C_PARAMETER_ACTION_PORT
#                 C_PARAMETER_ACTION_USER
#                 C_PARAMETER_ACTION_TO_EXECUTE
#                 C_PARAMETER_ACTION_RETRIES
#                 C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_remote_action_at_the_time()
{
  local err=0
  local output=""
  local try=0
  local config_values=""

  config_values="$config_values C_PERIODIC_ACTION "
  config_values="$config_values C_NEED_TO_RUN_AT_THE_TIME "
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ACTION_IP "
  config_values="$config_values C_PARAMETER_ACTION_PORT "
  config_values="$config_values C_PARAMETER_ACTION_USER "
  config_values="$config_values C_PARAMETER_ACTION_TO_EXECUTE "
  config_values="$config_values C_PARAMETER_ACTION_RETRIES "
  config_values="$config_values C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the action [$C_NAME_OF_ACTION]."
  log "This action does [$C_DESCRIPTION_OF_ACTION]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  # Verified this value C_RUN_AT_THE_TIME and C_RUN_AT_THE_TIME_DELTA_SECONDS ...
  is_the_time
  test $? -eq 0 || return 0
  
  while test "$try" -lt "$C_PARAMETER_ACTION_RETRIES"
  do
    output=$(execute_remote_action 2>&1)
    err=$(expr $err + $?)

    log "execute_remote_action[name:$C_NAME_OF_ACTION][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running

    my_sleep "$C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done    

  echo -n $output

  touch "$C_STATUS_FILE"

  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_local_action
# description   : Execute the local action.
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ACTION
#
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_local_action()
{
  local err=0
  local config_values=""
  
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ACTION "

  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi  

  local output=$(cd $C_PATH_BIN && $C_BINARY_NAME $C_PARAMETER_ACTION 2>&1)
  err=$(expr $err + $?)
    
  log "Execute the action [$C_NAME_OF_ACTION]."
  log "This action does [$C_DESCRIPTION_OF_ACTION]."
  
  log "cd $C_PATH_BIN && $C_BINARY_NAME $C_PARAMETER_ACTION"
  log "$output"
  
  echo $output  

  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_local_action
# description   : Execute a periodic action. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#                   
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ACTION
#                 C_RUN_EVERY_X_SECONDS
#                 C_PERIODIC_ACTION
#                 C_STATUS_FILE
#                 C_PARAMETER_ACTION_RETRIES
#                 C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_local_action()
{
  local err=0
  local try=0
  local output=""
  local config_values=""
  
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ACTION "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_PERIODIC_ACTION "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ACTION_RETRIES "
  config_values="$config_values C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES "
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the action [$C_NAME_OF_ACTION]."
  log "This action does [$C_DESCRIPTION_OF_ACTION]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  while test "$try" -lt "$C_PARAMETER_ACTION_RETRIES"
  do
    output=$(execute_local_action 2>&1)
    err=$(expr $err + $?)

    log "execute_local_action[name:$C_NAME_OF_ACTION][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running

    my_sleep "$C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done
  
  touch "$C_STATUS_FILE"
  
  return $err
}
#######################################################################################

#######################################################################################
# name          : execute_periodic_local_action_at_the_time
# description   : Execute a periodic action at specific time. 
# parameters    : none.
# note          : Is needed to set the following config values before to call this function:
#
#                 C_PERIODIC_ACTION  
#                 C_NEED_TO_RUN_AT_THE_TIME
#                 C_NAME_OF_ACTION
#                 C_DESCRIPTION_OF_ACTION
#                 C_PATH_BIN
#                 C_BINARY_NAME
#                 C_PARAMETER_ACTION
#                 C_RUN_EVERY_X_SECONDS
#                 C_RUN_AT_THE_TIME
#                 C_RUN_AT_THE_TIME_DELTA_SECONDS
#                 C_STATUS_FILE
#                 
# return        : 0 for OK otherwise 1.
#######################################################################################
execute_periodic_local_action_at_the_time()
{
  local err=0
  local try=0
  local output=""
  local config_values=""

  config_values="$config_values C_PERIODIC_ACTION "  
  config_values="$config_values C_NEED_TO_RUN_AT_THE_TIME "
  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "
  config_values="$config_values C_PARAMETER_ACTION "
  config_values="$config_values C_RUN_EVERY_X_SECONDS "
  config_values="$config_values C_RUN_AT_THE_TIME "
  config_values="$config_values C_RUN_AT_THE_TIME_DELTA_SECONDS "
  config_values="$config_values C_STATUS_FILE "
  config_values="$config_values C_PARAMETER_ACTION_RETRIES "
  config_values="$config_values C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES "  
  
  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  log "Execute the action [$C_NAME_OF_ACTION]."
  log "This action does [$C_DESCRIPTION_OF_ACTION]."

  verify_last_execution_time
  test $? -eq 0 || return 0

  # Verified this value C_RUN_AT_THE_TIME and C_RUN_AT_THE_TIME_DELTA_SECONDS ...
  is_the_time
  test $? -eq 0 || return 0
  
  while test "$try" -lt "$C_PARAMETER_ACTION_RETRIES"
  do
    output=$(execute_local_action 2>&1)
    err=$(expr $err + $?)

    log "execute_local_action[name:$C_NAME_OF_ACTION][retry:$try]"
    log "$output"
    
    touch_flagfile_for_instance_running

    my_sleep "$C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES"
    
    try=$(expr "$try" + 1)
  done

  echo -n $output

  touch "$C_STATUS_FILE"

  return $err
}
#######################################################################################

