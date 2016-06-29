#!/bin/sh

main()
{
   local rc=0

   local adb_binary=$(find $HOME/ -perm /u+x -name "adb" -print -quit -type f | head -1 | awk '{printf "%s", $0}')

	local action='shell pm list packages -f'

   sudo $adb_binary $action; rc=$(expr $rc + $?)

   return $rc
}

main

exit $?
