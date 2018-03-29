#!/bin/bash
#
#
# WARNING: This script is meant to be run ONLY ON PSBS08
# (SPOTCheck's build server) and this is called by the Jenkins's jobs
# "SPOTCheck sprint" or "SPOTCheck trunk".
#

#######################################################################################
# name 			: log
# description 	: log a line received by parameter in the console add a new line to each 
#				  line logged.
# parameters	: $1 the line to log.
# return		: 0 if OK, otherwise 1.
# example of use: 
#				log "Hello, this a try."
#				$?  ==> 0  this value is produce by return call.
#######################################################################################
log()
{
   local line_to_log="$1"
   
   local prefix_to_log=$(date "+%Y.%m.%d_%H:%M:%S:%N:")

   echo "$prefix_to_log"" [$line_to_log]" 2>/dev/null

   return $?
}
#######################################################################################

#######################################################################################
# name 			: match
# description 	: find a pattern in string, if it found then this function returns the 
#				  index in the string where the pattern was found, it is between 1 to n.
# parameters	: $1 the string.
#				  $2 the pattern to find.
# return		: 0 if OK and print the index, otherwise 1.
# example of use: 
#				rc$(match "Where can i find a dog to buy?." "dog")
#				$?  ==> 0  this value is produce by return call.
#				$rc ==> 20 this is the index in the string 1-n
#######################################################################################
function match()
{
	local string="$1"
	
	local pattern="$2"
	
	local rc=$(awk -v string="$string" -v pattern="$pattern" 'BEGIN{ printf "%d", match(tolower(string), tolower(pattern)); }' 2>/dev/null)
	
	rc=$((($rc + 0)))
	
	printf $rc
	
	test $rc -gt 0 || return 1
	
	return 0 
}
#######################################################################################

#######################################################################################
# name 			: sizeof
# description 	: return the size in bytes of the file.
# parameters	: $1 the file.
# return		: 0 if OK and print the size of the file, otherwise 1.
# example of use: 
#				rc$(sizeof ./how_to_build.txt)
#				$?  ==> 0    this value is produce by return call.
#				$rc ==> 5393 this is the size of the file in bytes.
#######################################################################################
function sizeof()
{
	local filePath="$1"
	
	local sizeBytes=$(wc -c $filePath 2>/dev/null | awk '{printf "%s", $1}')
	
	sizeBytes=$((($sizeBytes + 0)))
	
	printf $sizeBytes
	
	test $sizeBytes -ge 0 || return 1
	
	return 0
}
#######################################################################################

#######################################################################################
# name 			: fileCopy
# description 	: copy a file in a secure form, this function verify if the copy was ok 
#				  and if there was an error retry the copy until to complete the total 
#				  of retries. This function is useful when is needed to copy a file to
#				  network filesystem.
# parameters	: $1 file to copy.
#				  $2 directory where the file will be put.
#				  $3 retries.
# return		: 0 if OK, otherwise 1.
# example of use: 
#				fileCopy $PATH_M3_SRC/spotcheck $PATH_M3_ARTIFACT
#				$?  ==> 0 this value is produce by return call.
#######################################################################################
function fileCopy
{
	local origin="$1"
	
	local target="$2"
	
	local retries=$( (test -n "$3" && printf "$3") || echo "4")
	
	test -s $origin -a -r $origin || return 1
	
	test -w $target -a -d $target || return 1
	
	local i=1
	
	local rc=0
	
	local file=$(basename $origin 2>/dev/null)
	
	cp --force $origin $target 2>/dev/null
	
	log "Copying file [$origin] to [$target/$file]"
	
	rc=$? 
	
	local sizeOrigin=$(sizeof $origin)
	
	local sizeTarget=$(sizeof $target/$file)
	
	while [ $sizeOrigin -ne $sizeTarget ] && [ $i -le $retries ]
	do
		log "retry [$i] to copy file [$origin] to [$target/$file]"
		
		cp --force $origin $target 2>/dev/null
		
		rc=$?
		
		sizeTarget=$(sizeof $target/$file)
		
		i=$(expr $i + 1)
	done
	
	sizeTarget=$(sizeof $target/$file)
	
	test $sizeOrigin -ne $sizeTarget && return 1 	
	
	return 0
}
#######################################################################################

# Check the received parameters.
#######################################################################################
function check_parameters()
{
	local index=$(match "$1" "sprint")
	
	if test $? -eq 0 -a $index -gt 0
	then
		printf "sprint"
	else
		printf "trunk"
	fi
	
	return 0
}
#######################################################################################

#######################################################################################
function build_path_artifact_for_trunk_using_sprint_number()
{
	local SPRINT_NUMBER=1
	
	if [ "$BRANCH" == "trunk" ] && [ -d "$PATH_SRC" ]
	then
		#The version number must be in below form at $PATH_SRC/app/spotcheck/Version/version.cpp file.
		#char gszVersion[] = " V00.00.XX ";
		#If this line is changed you need to change the following subshell call.
		SPRINT_NUMBER=$(cat $PATH_SRC/app/spotcheck/Version/version.cpp | grep gszVersion | sed 's/\"//g' | sed 's/\;//g' | sed 's/\([a-zA-Z]\)//g' | awk -F= '{print $2}' | sed 's/ //g' 2>/dev/null)
		#######
	
		if [ $? -ne 0 ] || [ -z "$SPRINT_NUMBER" ] 
		then
			log "ERROR: Unable to locate version file [$PATH_SRC/app/spotcheck/Version/version.cpp], sprint number [$SPRINT_NUMBER]."
	    	return 1
	    fi
	
	    PATH_ARTIFACT="$PATH_ARTIFACT"/"relt_sprint_"$SPRINT_NUMBER
	
	    [ -d "$PATH_ARTIFACT" ] || mkdir -p "$PATH_ARTIFACT"
		if [ $? -ne 0 ] || [ ! -d "$PATH_ARTIFACT" ]
		then
			log "ERROR: Unable to create sprint dir [$PATH_ARTIFACT] to copy artifacts."
	    	return 2
	    fi
	fi

	return 0
}
#######################################################################################

#######################################################################################
function quit()
{
	reset
	log "Interrupt signal received, quiting ..."
	exit 0
}
#######################################################################################

#######################################################################################
function check_or_create_directory()
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
#######################################################################################

#######################################################################################
function check_directory()
{
	local directory=$1
	
	if [ ! -d "$directory" ] || [ ! -w "$directory" ]
	then
		log "ERROR: Unable to locate folder [$directory]."
		return 1
	fi
	
	return 0
}
#######################################################################################

#######################################################################################
function check_working_dirs
{
	local rc=0
	 
	log "**********************************************************************"
	log "*                Begin checking SPOTCheck's folder tree              *"
	log "**********************************************************************"
	
	check_directory "$PATH_SRC"											; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_M3_ARTIFACT"					; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_M5_ARTIFACT"					; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_VGD_ARTIFACT"					; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_WINDOWS_ARTIFACT"				; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_WINDOWS_ARTIFACT_INSTALLER"; rc=$(expr $rc + $?)
	check_or_create_directory "$PATH_LINUX_ARTIFACT"				; rc=$(expr $rc + $?)
	
	log "**********************************************************************"
	log "*                End checking SPOTCheck's folder tree                *"
	log "**********************************************************************"	
	
	return $rc
}
#######################################################################################

#######################################################################################
function setup()
{
	BRANCH=$(check_parameters "$1")
	PATH_BASE=$(pwd)"/../.."
	PATH_SRC="$PATH_BASE/$BRANCH/src"
	PATH_ARTIFACT="$PATH_BASE/$BRANCH/artifacts"
	
	# For spotcheck application.
	PATH_M3_SRC="$PATH_SRC/.build_spotm3/app/spotcheck"
	PATH_M5_SRC="$PATH_SRC/.build_spotm5/app/spotcheck"
	PATH_VGD_SRC="$PATH_SRC/.build_vgd/app/spotcheck"
	PATH_WINDOWS_SRC="$PATH_SRC/.build_windows/app/spotcheck"
	PATH_LINUX_SRC="$PATH_SRC/.build_linux/app/spotcheck"

	# For spotdashboard application.
	PATH_M3_SPOTDASHBOARD_SRC="$PATH_SRC/.build_spotm3/app/spotdashboard"
	PATH_M5_SPOTDASHBOARD_SRC="$PATH_SRC/.build_spotm5/app/spotdashboard"
	PATH_WINDOWS_SPOTDASHBOARD_SRC="$PATH_SRC/.build_windows/app/spotdashboard"
	PATH_LINUX_SPOTDASHBOARD_SRC="$PATH_SRC/.build_linux/app/spotdashboard"

	# Trap any signal between 1-30 and call quit function to log and exit cleanly.
	trap quit 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30

	build_path_artifact_for_trunk_using_sprint_number
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted!!!"
		return 1
	fi
	
	# For both applications.
	PATH_M3_ARTIFACT="$PATH_ARTIFACT/spot_M3/exe"
	PATH_M5_ARTIFACT="$PATH_ARTIFACT/spot_M5/exe"
	PATH_VGD_ARTIFACT="$PATH_ARTIFACT/spot_VGD/exe"
	PATH_WINDOWS_ARTIFACT="$PATH_ARTIFACT/windows/exe"
	PATH_WINDOWS_ARTIFACT_INSTALLER="$PATH_ARTIFACT/windows/installer"
	PATH_LINUX_ARTIFACT="$PATH_ARTIFACT/linux/exe"

	


	check_working_dirs
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted!!!"
		return 2
	fi

	return 0
}
#######################################################################################

#######################################################################################
function main()
{
	setup $*
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted!!!"
		exit 1
	fi	
	
	cd $PATH_SRC 2>/dev/null || ( log "ERROR to changing directory [$PATH_SRC]." && exit 2 )
	
	build_m3
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted for M3.!!!"
		exit 2
	fi	
	
	build_m5
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted for M5.!!!"
		exit 3
	fi	

	build_vgd
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted for VGD.!!!"
		exit 6
	fi	
	
	build_windows
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted for WINDOWS.!!!"
		exit 4
	fi	
	
	build_linux
	if [ $? -ne 0 ]
	then
		log "ERROR: Build aborted for LINUX.!!!"
		exit 5
	fi	


	log "**********************************************************************"
	log "*           SPOTCheck successfully built for 4 platforms!            *"
	log "**********************************************************************"	
        cd ROMfs
        ./versions.sh

        for i in 5 6 7 8
        do
                ./makePackages.sh $i
                ./makePackages.sh $i m3 Prod
                ./makePackages.sh $i m5
                ./makePackages.sh $i m5 Prod
        done

        cd ..
	
	exit 0
}
#######################################################################################

#######################################################################################
function build_m3()
{
	local rc=0
	
	log "**********************************************************************"
	log "*            Begin building SPOTCheck for SPOT M3 platform           *"
	log "**********************************************************************"
	export PATH=/usr/local/m68k-uclinux-tools/bin:/usr/local/bin:/usr/bin:/bin
	./build -3
	if [ $? -ne 0 ]
	then
	    log "ERROR building for SPOT M3 platform."
	    return 1
	fi
	
	fileCopy $PATH_M3_SRC/spotcheck $PATH_M3_ARTIFACT								; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTCheck.ini $PATH_M3_ARTIFACT								; rc=$(expr $rc + $?)
	
	fileCopy $PATH_M3_SPOTDASHBOARD_SRC/spotdashboard $PATH_LINUX_ARTIFACT	; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTDashboard.ini $PATH_LINUX_ARTIFACT					; rc=$(expr $rc + $?)	
	
	log "**********************************************************************"
	log "*            End building SPOTCheck for SPOT M3 platform             *"
	log "**********************************************************************"
	
	return $rc
}
#######################################################################################

#######################################################################################
function build_m5()
{
	local rc=0
	
	log "**********************************************************************"
	log "*            Begin building SPOTCheck for SPOT M5 platform           *"
	log "**********************************************************************"
	export PATH=/opt/arm_lite/bin:/usr/local/bin:/usr/bin:/bin
	./build -5
	if [ $? -ne 0 ]
	then
	    log "ERROR building for SPOT M5 platform."
	    return 1
	fi
	
	fileCopy $PATH_M5_SRC/spotcheck $PATH_M5_ARTIFACT								; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTCheck.ini $PATH_M5_ARTIFACT								; rc=$(expr $rc + $?)
	
	fileCopy $PATH_M5_SPOTDASHBOARD_SRC/spotdashboard $PATH_LINUX_ARTIFACT	; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTDashboard.ini $PATH_LINUX_ARTIFACT					; rc=$(expr $rc + $?)	
	
	log "**********************************************************************"
	log "*            End building SPOTCheck for SPOT M5 platform             *"
	log "**********************************************************************"
	
	return $rc
}
#######################################################################################

#######################################################################################
function build_vgd()
{
	local rc=0
	
	log "**********************************************************************"
	log "*            Begin building SPOTCheck for SPOT VGD platform           *"
	log "**********************************************************************"
	export PATH=/opt/arm_lite/bin:/usr/local/bin:/usr/bin:/bin
	./build -j
	if [ $? -ne 0 ]
	then
	    log "ERROR building for SPOT VGD platform."
	    return 1
	fi
	
	fileCopy $PATH_M5_SRC/spotcheck $PATH_VGD_ARTIFACT								; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTCheck.ini $PATH_VGD_ARTIFACT								; rc=$(expr $rc + $?)
	
	fileCopy $PATH_M5_SPOTDASHBOARD_SRC/spotdashboard $PATH_LINUX_ARTIFACT	; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTDashboard.ini $PATH_LINUX_ARTIFACT					; rc=$(expr $rc + $?)	
	
	log "**********************************************************************"
	log "*            End building SPOTCheck for SPOT VGD platform             *"
	log "**********************************************************************"
	
	return $rc
}
#######################################################################################

#######################################################################################
function build_windows()
{
	local rc=0
	
	log "**********************************************************************"
	log "*         Begin building SPOTCheck for SPOT Windows platform         *"
	log "**********************************************************************"
	export PATH=/usr/bin:/bin
	./build -w
	if [ $? -ne 0 ]
	then
	    log "ERROR building for Windows platform"
	    return 1
	fi
	
	fileCopy $PATH_WINDOWS_SPOTDASHBOARD_SRC/spotdashboard.exe $PATH_WINDOWS_ARTIFACT							; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/installer/SPOTDashboard.ini $PATH_WINDOWS_ARTIFACT											; rc=$(expr $rc + $?)	
	
	fileCopy $PATH_WINDOWS_SRC/spotcheck.exe $PATH_WINDOWS_ARTIFACT													; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/installer/SPOTCheck.ini $PATH_WINDOWS_ARTIFACT													; rc=$(expr $rc + $?)
	
	./installer/innosetup_console.sh ./installer/SPOTCheck_windows.iss												; rc=$(expr $rc + $?)

	OUTPUT_BASE_FILENAME=`grep 'OutputBaseFilename=' ./installer/SPOTCheck_windows.iss | cut -d = -f 2 | cut -c 1-28`
	fileCopy $PATH_SRC/installer/target/${OUTPUT_BASE_FILENAME}.exe $PATH_WINDOWS_ARTIFACT_INSTALLER	; rc=$(expr $rc + $?)
	
	log "**********************************************************************"
	log "*         End building SPOTCheck for SPOT Windows platform           *"
	log "**********************************************************************"
	
	return $rc
}
#######################################################################################

#######################################################################################
function build_linux()
{
	local rc=0
	
	log "**********************************************************************"
	log "*          Begin building SPOTCheck for SPOT linux platform          *"
	log "**********************************************************************"
	export PATH=/usr/bin:/bin
	./build -t
	if [ $? -ne 0 ]
	then
	    log "ERROR building for Linux platform"
	    return 1
	fi
	
	fileCopy $PATH_LINUX_SRC/spotcheck $PATH_LINUX_ARTIFACT							; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTCheck.ini $PATH_LINUX_ARTIFACT								; rc=$(expr $rc + $?)
	
	fileCopy $PATH_LINUX_SPOTDASHBOARD_SRC/spotdashboard $PATH_LINUX_ARTIFACT	; rc=$(expr $rc + $?)
	fileCopy $PATH_SRC/SPOTDashboard.ini $PATH_LINUX_ARTIFACT						; rc=$(expr $rc + $?)	
	
	
	log "**********************************************************************"
	log "*          End building SPOTCheck for SPOT linux platform            *"
	log "**********************************************************************"
	
	return $rc
}
#######################################################################################

#######################################################################################
main $*
#######################################################################################
