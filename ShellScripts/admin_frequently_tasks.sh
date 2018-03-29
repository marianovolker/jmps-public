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
# name          : fix_devices_permissions
# description   : Fix devices permissions.
# parameters    : none
# return        : 0 if OK, otherwise distinct of 0.
#######################################################################################
fix_devices_permissions()
{
  local err=0
  local required_permission=666
  local current_permission=0
  
  # Correct files permissions of usb devices like ttyUSB*
  for inode in $(find /dev/ -name "ttyUSB*" -type c)
  do
    current_permission=$(stat -c "%a" $inode); err=$(expr $err + $?)
    if test "$current_permission" != "$required_permission"
    then
      chmod "$required_permission" "$inode"; err=$(expr $err + $?)
      log "file permission of [$inode] was fixed."
    fi
  done

  # Correct files permissions of video devices like /dev/video*
  for inode in $(find /dev/ -name "video*" -type c)
  do
    current_permission=$(stat -c "%a" $inode); err=$(expr $err + $?)
    if test "$current_permission" != "$required_permission"
    then
      chmod "$required_permission" "$inode"; err=$(expr $err + $?)
      log "file permission of [$inode] was fixed."
    fi
  done
  
  return $err
}
#######################################################################################

#######################################################################################
# name          : verify_if_exist_a_command_to_execute
# description   : Verify if exist a command to execute as root.
# parameters    : none
# return        : 0 if OK, otherwise distinct of 0.
#######################################################################################
verify_if_exist_a_command_to_execute()
{
  local err=0
  
  test -z "$C_SYSTEM_CMDS_FILENAME" && export C_SYSTEM_CMDS_FILENAME="/opt/jmps/cmds/to_execute.lst"
  
  test ! -r "$C_SYSTEM_CMDS_FILENAME" && log "File $C_SYSTEM_CMDS_FILENAME not found." && return 0
  
  local action_to_execute=$(cat "$C_SYSTEM_CMDS_FILENAME" | tail -1 2>/dev/null); err=$(expr $err + $?)
  log "action_to_execute; err=$err"
  
  local cmd_to_execute=""
  local list_of_actions_allowed="reboot | reset_camaras | find_alarms_actions"
  local rc=$(match "$list_of_actions_allowed" "$action_to_execute")
  
  if test $? -eq 0
  then
    # the action is allowed to be executed ...
    if test "$action_to_execute" = "reboot"
    then
      cmd_to_execute="/sbin/reboot"
      
    elif test "$action_to_execute" = "reset_camaras"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/reset_cameras.sh"
      
    elif test "$action_to_execute" = "make_system_backup"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/make_backup_of_config_files.sh manual"
            
    elif test "$action_to_execute" = "find_alarms_actions"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/find_actions_and_alarms.sh"
      
    elif test "$action_to_execute" = "restart_web_server"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/restart_web_server.sh"
      
    elif test "$action_to_execute" = "start_admin_tasks"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/start_admin_tasks.sh"      
      
    elif test "$action_to_execute" = "stop_admin_tasks"
    then
      cmd_to_execute="sh /opt/jmps/bin/etc/stop_admin_tasks.sh"
      
    fi
  fi
  
  unlink "$C_SYSTEM_CMDS_FILENAME" 2>/dev/null 1>&2; err=$(expr $err + $?)
  log "unlink $C_SYSTEM_CMDS_FILENAME; err=$err"
  
  if test "$cmd_to_execute" != ""
  then
    log "Command to execute [$cmd_to_execute]"
    nohup sleep 1 && env -i $cmd_to_execute 2>/dev/null 1>&2 &
    err=$(expr $err + $?)
  else
    log "[action_to_execute=$action_to_execute] not valid ..."
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
  
  test -z "$C_TIME_TO_RUN_ADMIN_FRECUENTLY_TASKS" && export C_TIME_TO_RUN_ADMIN_FRECUENTLY_TASKS=80
  
  while test "$err" -eq 0
  do

    log "fix_devices_permissions"
    fix_devices_permissions
    err=$(expr $err + $?)

    log "verify_if_exist_a_command_to_execute"
    verify_if_exist_a_command_to_execute
    err=$(expr $err + $?)
    
    touch_flagfile_for_instance_running

    my_sleep $C_TIME_TO_RUN_ADMIN_FRECUENTLY_TASKS
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

