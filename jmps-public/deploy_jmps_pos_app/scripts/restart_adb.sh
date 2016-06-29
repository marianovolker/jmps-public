#!/bin/sh

main()
{
	local rc=0
   local adb_binary=$(find $HOME/ -type f -perm /u+x -name "adb" -print -quit 2>/dev/null | head -1 | awk '{printf "%s", $0}')

	sudo $adb_binary kill-server; rc=$(expr $rc + $?)
	sudo $adb_binary start-server; rc=$(expr $rc + $?)
	sudo $adb_binary devices; rc=$(expr $rc + $?)

	return $rc
}

main

exit $?
