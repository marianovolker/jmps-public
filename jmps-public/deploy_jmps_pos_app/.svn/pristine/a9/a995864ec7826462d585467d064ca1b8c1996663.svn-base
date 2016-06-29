
adb_binary=$(find $HOME/ -perm /u+x -name "adb" -print -quit -type f | head -1 | awk '{printf "%s", $0}')

for i in $(sh list_of_packages.sh | tr -d '\r' | awk -F: '{print $2}' | grep "/data/" | awk -F= '{printf "%s\n",$2}')
do 
	echo "uninstalling [$i] from the device."
	echo "$adb_binary uninstall $i"
	echo "[$i]"
	$adb_binary uninstall $i
done
