##-#######################################################################-##
# Copyright © 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file GvrRules_m3.cmake
## @author pcantarini
## @date Monday, February 09, 2015
## @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

##-#################################################################################-##
# Overwrite variable <cmake>/Modules/Platform/Linux-GNU.cmake
# unfortunately we have to do this as the Linux-GNU cross-platform cmake file harcode
# the '-rdynamic' flag
# this harcoded '-rdynamic' flag has been reported several times as a defect in cmake
#set(__LINUX_COMPILER_GNU 1)
#macro(__linux_compiler_gnu lang)
#  # remove the hardcoded "-rdynamic" from the 'CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS'
#  set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS "")
#endmacro()
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
##-#################################################################################-##

# necessary flags for compilation and linking on this platform
# the 'CMAKE_*_FLAGS_INIT' version of these variables are used instead of the 'CMAKE_*_FLAGS' versions as these ones are overwritten and the _INIT ones are appended
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -D__linux__ -Wstrict-prototypes -Wall -Wno-trigraphs -fno-strict-aliasing -mno-strict-align -fno-builtin -w -msoft-float -Os -m5307 -fno-common -fexceptions -ftemplate-depth-1024")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -D__linux__ -Wall -Wno-trigraphs -fno-strict-aliasing -mno-strict-align -fno-builtin -w -msoft-float -Os -m5307 -fno-common -fexceptions -ftemplate-depth-1024")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -m5307 -static -Wl,-elf2flt='-s 32768 -z' -Wl,-move-rodata -lc")

