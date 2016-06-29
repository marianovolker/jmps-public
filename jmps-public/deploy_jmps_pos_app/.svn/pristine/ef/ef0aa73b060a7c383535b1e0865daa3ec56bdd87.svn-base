#!/bin/sh

adb_binary=""

log()
{
   local line_to_log="$1"

   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null

   return $?
}

list_of_packages()
{
   local rc=0

	[ -n "$adb_binary" ] || adb_binary=$(find $HOME/ -perm /u+x -name "adb" -print -quit -type f | head -1 | awk '{printf "%s", $0}')

	local action='shell pm list packages -f'

   sudo $adb_binary $action; rc=$(expr $rc + $?)

   return $rc
}

check_or_create_directory()
{
   local directory=$1

   [ -d "$directory" ] || mkdir -p "$directory" 2>/dev/null
   if [ $? -ne 0 ] || [ ! -d "$directory" ] || [ ! -w "$directory" ]
   then
      log "ERROR: Unable to locate/create folder [$directory]."
      return 1
   fi

   return 0
}

backup_of_packages()
{
	local date_directory=$(date "+%Y.%m.%d")
	local directory=""

	[ -n "$adb_binary" ] || adb_binary=$(find $HOME/ -type f -perm /+x -name "adb" -print -quit | head -1 | awk '{printf "%s", $0}')

	directory="backup_device_$date_directory/system/app"
	check_or_create_directory "$directory"
	for i in $(list_of_packages | awk -F: '{print $2}' | awk -F= '{print $1}' | grep "/system/")
	do
		log "getting this [$i] system package from the device and put into [$directory/$i]..."
		$adb_binary pull $i $directory/$i
	done

	directory="backup_device_$date_directory/data/app"
	check_or_create_directory "$directory"
	for i in $(list_of_packages | awk -F: '{print $2}' | awk -F= '{print $1}' | grep "/data/")
	do
		log "getting this [$i] data package from the device and put into [$directory/$i]..."
		$adb_binary pull $i $directory/$i
	done

	return 0
}

main()
{
	backup_of_packages

   if [ $? -ne 0 ]
   then
      log "ERROR: Backup aborted!!!"
      return 1
   fi

	return 0
}


main

exit $?
