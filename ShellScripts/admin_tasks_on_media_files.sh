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
# name          : convert_avi_to_webm
# description   : This function convert an avi video file to webm for reproduce with html5.
# parameters    : $1 the inode that represent the video file.
# return        : C_OK if OK, otherwise C_NOK.
#######################################################################################
convert_avi_to_webm()
{
  local inode="$1"

  local rc=0

  local size=0 

  check_entry "$inode"
  test $? -ne $C_OK && log "Bad inode [$inode]" && return $C_NOK

  local extension=$(get_inode_extension "$inode"); rc=$(expr $rc + $?)

  local inode_name=$(get_inode_name "$inode"); rc=$(expr $rc + $?)

  if test "$extension" != "avi" -o $rc -ne 0
  then
    return $C_NOK
  fi

  size=$(sizeof "$inode")
  if ! test "$size" -gt 0
  then
    log "Discarding [1][the file has zero size][$inode]"
    log "Deleting the inode [$inode].[rm -f \"$inode\"]"
    rm -f "$inode"
    return $C_OK
  fi

  # Then we do the conversion...
  if ! test -f "$inode_name"".webm"
  then
    log "Begining to process this inode:[$inode]"
    yes | avconv -i "$inode" "$inode_name"".webm" 2>/dev/null 1>&2; rc=$(expr $rc + $?)
    log "Finished the processing of this inode:[$inode]"
  else
    log "Ignoring this inode:[$inode]"
  fi

  test $rc -eq 0 && rm -rf "$inode"; rc=$(expr $rc + $?)

  test $rc -eq 0 && return $C_OK

  return $C_NOK
}
#######################################################################################

#######################################################################################
# name          : convert_recorded_files
# description   : This function convert all video files.
# parameters    : none.
# return        : C_OK if OK, otherwise C_NOK.
#######################################################################################
convert_recorded_files()
{
  local err=0
  local index=1
  local number=""
  local camera_dir=""
  local time_last_modif=0
  local inode=""
  local entry=""
  local wait_time_since_last_modif_in_minutes=1

  while test $index -le $C_QUANTITY_OF_VIDEO_DEVICES
  do
    number=$(printf "%02d" $index)
    camera_dir="$C_PATH_FOR_VIDEO_DEVICES""stream$number"

    log "converting video files at $camera_dir"

    for entry in $camera_dir/*
    do
      check_dir_entry "$entry"
      if test $? -eq $C_TRUE
      then
        for inode in $(find "$entry/" -name "*.avi" -type f)
        do
          time_last_modif=$(get_time_from_last_inode_modification "$inode")
          
          log "-"
          
          if test "$time_last_modif" -ge "$wait_time_since_last_modif_in_minutes"
          then
            log "Converting this inode [$inode] from avi to webm."
            convert_avi_to_webm "$inode"; err=$(expr $err + $?)
          else
            log "Ignoring this inode [$inode] because is still been recording. time_last_modif=[$time_last_modif]."
          fi
          
          log "-"
          
          # Update the flagfile to indicate this script is still alive.
          touch_flagfile_for_instance_running "$0"
        done
      fi
      err=$(expr $err + $?)
    done

    log "*"
    index=$(expr $index + 1)
  done

  test $err -eq 0 && return $C_OK

  return $C_NOK
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
  local current_path=""
  local str=""

  current_path=$(get_current_path)
  err=$(expr $err + $?)
  test $err -ne 0 && str="Error building actual path [$current_path]. Exiting..." && _exit $err "$str" 

  # We need to do the following things in the correct order, so please do not change this order.

  # First we need to sort the recorded files from cameras ...
  sh "$current_path""organize_files_of_recordings.sh"; err=$(expr $err + $?)

  # Second we need to sort the pictures from cameras ...
  sh "$current_path""organize_files_of_snapshot_photos.sh"; err=$(expr $err + $?)
  
  # Third we need to erase older files ...
  sh "$current_path""erase_old_recorded_files.sh"; err=$(expr $err + $?)

  # Fourth we need to convert recorded files from avi to webm to play better with html5.
  convert_recorded_files; err=$(expr $err + $?)
  #------------------------------------------------------------------------------------

  #------------------------------------------------------------------------------------
  _exit $err "$str"
  #------------------------------------------------------------------------------------
}
#######################################################################################

#######################################################################################
main
#######################################################################################


