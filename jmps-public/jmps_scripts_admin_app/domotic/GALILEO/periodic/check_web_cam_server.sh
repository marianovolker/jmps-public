#!/bin/sh

#######################################################################################
C_JMPS_LIB_FILE="/scripts_jmps/jmps.lib.sh"
. $C_JMPS_LIB_FILE 2>/dev/null
test $? -ne 0 && echo "ERROR: loading library file, it must located in [$C_JMPS_LIB_FILE]" && exit 1
#######################################################################################

#######################################################################################
debug=0
test $debug -eq 1 && set -x
#######################################################################################

#######################################################################################
server_to_check_port=2
server_to_check_ip=192.168.1.10
max_retries=30
command_to_execute="ping -W 10 -c 1 $server_to_check_ip"

log "........................................................................................."
log "Starting ..."
log "server_to_check_port=[$server_to_check_port]"
log "server_to_check_ip=[$server_to_check_ip]"
log "max_retries=[$max_retries]"
log "command_to_execute=[$command_to_execute]"
log "begin..."

retry=1
log "executing [$command_to_execute]"
$command_to_execute 2>/dev/null 1>&2
rc=$?

while test "$rc" -ne 0
do

  log "Fail ping ..."
  log "executing [$command_to_execute]"
  $command_to_execute 2>/dev/null 1>&2
  rc=$?

  if test "$rc" -ne 0 -a "$retry" -gt "$max_retries"
  then
    log "executing poweroff [sh /scripts_jmps/digital_port_off.sh "$server_to_check_port";rc_1=$?]..."
    sh /scripts_jmps/digital_port_off.sh "$server_to_check_port";rc_1=$?

    sleep 10

    log "executing poweron [sh /scripts_jmps/digital_port_on.sh "$server_to_check_port";rc_2=$?]..."
    sh /scripts_jmps/digital_port_on.sh "$server_to_check_port";rc_2=$?

    log "end..."
    log "........................................................................................."
    exit 1
  fi

  retry=$(expr "$retry" + 1)
done

log "end..."
log "........................................................................................."

exit $rc

