#!/bin/sh

#######################################################################################
# name          : init_action
# description   : Initialize the action and load all libraries. 
# parameters    : none.
# return        : 0 for OK otherwise 1.
#######################################################################################
init_action()
{
  local DIR=$(dirname $0)

  C_JMPS_CONFIG_FILE=$DIR"/../../../config/etc/jmps.conf"
  C_CONFIG_FILE=$DIR"/../config/action.conf"  
  C_JMPS_LIB_FILE=$DIR"/../../../libs/jmps.lib.sh"
  C_JMPS_ACTIONS_LIB_FILE=$DIR"/../../../libs/jmps.actions.lib.sh"
  C_PATH_LOG=$DIR"/../logs/"
  C_PATH_BIN=$DIR"/../../../bin/eys/"
  C_BINARY_NAME="./eys_multipurpose_controller"
  C_PARAMETER_ACTION="pulse"
  
  test -r $C_JMPS_CONFIG_FILE && . $C_JMPS_CONFIG_FILE            ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_CONFIG_FILE]" && exit 1
  test -r $C_CONFIG_FILE && . $C_CONFIG_FILE                      ; test $? -ne 0 && echo "ERROR: loading [$C_CONFIG_FILE]" && exit 1  
  test -r $C_JMPS_LIB_FILE && . $C_JMPS_LIB_FILE                  ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_LIB_FILE]" && exit 1
  test -r $C_JMPS_ACTIONS_LIB_FILE && . $C_JMPS_ACTIONS_LIB_FILE  ; test $? -ne 0 && echo "ERROR: loading [$C_JMPS_ACTIONS_LIB_FILE]" && exit 1
  test -x "$C_PATH_BIN""$C_BINARY_NAME"                           ; test $? -ne 0 && echo "ERROR: is not executable by current user ["$C_PATH_BIN""$C_BINARY_NAME"]" && exit 1
  test -d $C_PATH_LOG                                             ; test $? -ne 0 && echo "ERROR: no es un directorio valido [$C_PATH_LOG]" && exit 1
  test -d $C_PATH_BIN                                             ; test $? -ne 0 && echo "ERROR: no es un directorio valido [$C_PATH_BIN]" && exit 1

  log "-------------------------------------------------------------------------------"
      
  test $C_DEBUG -eq 1 && set -x

  local config_values=""

  config_values="$config_values C_NAME_OF_ACTION "
  config_values="$config_values C_DESCRIPTION_OF_ACTION "
  config_values="$config_values C_PARAMETER_ACTION "
  config_values="$config_values C_PATH_BIN "
  config_values="$config_values C_BINARY_NAME "

  if test "$C_PERIODIC_ACTION" = "1"
  then
    if test "$C_PERIODIC_ONLY" = "1"
    then
      config_values="$config_values C_RUN_EVERY_X_SECONDS "
      config_values="$config_values C_PARAMETER_ACTION_RETRIES "
      config_values="$config_values C_PARAMETER_ACTION_DELAY_IN_SECONDS_BETWEEN_RETRIES "      
      
    elif test "$C_NEED_TO_RUN_AT_THE_TIME" = "1"
    then
      config_values="$config_values C_RUN_AT_THE_TIME "
      config_values="$config_values C_RUN_AT_THE_TIME_DELTA_SECONDS "
      config_values="$config_values C_RUN_EVERY_X_SECONDS "

    fi

    config_values="$config_values C_STATUS_FILE "
  fi

  verify_config_values "$config_values"
  if test $? -ne 0
  then
    log "One of the following config variables was not set."
    return 1
  fi
  
  # If this action is not enabled then exit without an error condition ...
  test "$C_ENABLE_ACTION" != "1" && log "This action [$C_NAME_OF_ACTION] is disabled by configuration ..." && exit 0  

  C_FLAGFILE_NAME="$0""_""$C_NAME_OF_ACTION"

  # If there exist another instance running interrupt the execution of this script.
  check_if_there_is_another_instance_running

  # Trap any signal between 1-30 and call quit function to log and exit cleanly.
  trap quit_action 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30  
}
#######################################################################################

#######################################################################################
main()
{
  #----------------------------------------------------------------------------------
  init_action
  #----------------------------------------------------------------------------------

  #----------------------------------------------------------------------------------
  local err=0
  local action="$1"
  local str=""
  local output=""

  case "$action" in
    "pulse")
      output=$(execute_local_action)
      err=$(expr $err + $?)
    ;;
    "auto")
      if test "$C_PERIODIC_ACTION" = "1"
      then
        if test "$C_PERIODIC_ONLY" = "1"
        then
          output=$(execute_periodic_local_action)
        elif test "$C_NEED_TO_RUN_AT_THE_TIME" = "1"
        then
          output=$(execute_periodic_local_action_at_the_time)
        fi    
      fi

      err=$(expr $err + $?)
      log "output=[$output]"        
    ;;
    *)
      str="invalid parameter [$action]."
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

