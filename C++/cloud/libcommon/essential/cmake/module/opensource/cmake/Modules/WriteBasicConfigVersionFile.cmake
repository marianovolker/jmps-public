#  WRITE_BASIC_CONFIG_VERSION_FILE( filename VERSION major.minor.patch COMPATIBILITY (AnyNewerVersion|SameMajorVersion) )
#
# Deprecated, see WRITE_BASIC_PACKAGE_VERSION_FILE(), it is identical.

#=============================================================================
# Copyright 2008-2011 Alexander Neundorf, <neundorf@kde.org>
# Copyright 2004-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)
#=============================================================================
# Copyright.txt:
# 
# CMake - Cross Platform Makefile Generator
# Copyright 2000-2011 Kitware, Inc., Insight Software Consortium
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# ------------------------------------------------------------------------------
# 
# The above copyright and license notice applies to distributions of
# CMake in source and binary form.  Some source files contain additional
# notices of original copyright by their contributors; see each source
# for details.  Third-party software packages supplied with CMake under
# compatible licenses provide their own copyright notices documented in
# corresponding subdirectories.
# 
# ------------------------------------------------------------------------------
# 
# CMake was initially developed by Kitware with the following sponsorship:
# 
#  * National Library of Medicine at the National Institutes of Health
#    as part of the Insight Segmentation and Registration Toolkit (ITK).
# 
#  * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel
#    Visualization Initiative.
# 
#  * National Alliance for Medical Image Computing (NAMIC) is funded by the
#    National Institutes of Health through the NIH Roadmap for Medical Research,
#    Grant U54 EB005149.
# 
#  * Kitware, Inc.
#=============================================================================

include(CMakeParseArguments)

function(WRITE_BASIC_CONFIG_VERSION_FILE _filename)

  set(options )
  set(oneValueArgs VERSION COMPATIBILITY )
  set(multiValueArgs )

  cmake_parse_arguments(CVF "${options}" "${oneValueArgs}" "${multiValueArgs}"  ${ARGN})

  if(CVF_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unknown keywords given to WRITE_BASIC_CONFIG_VERSION_FILE(): \"${CVF_UNPARSED_ARGUMENTS}\"")
  endif()

# GVR
  find_file(versionTemplateFile "BasicConfigVersion-${CVF_COMPATIBILITY}.cmake.in" HINTS "${CMAKE_ROOT}/Modules" ${CMAKE_MODULE_PATH} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
#  set(versionTemplateFile "${CMAKE_ROOT}/Modules/BasicConfigVersion-${CVF_COMPATIBILITY}.cmake.in")
  if(NOT EXISTS "${versionTemplateFile}")
    message(FATAL_ERROR "Bad COMPATIBILITY value used for WRITE_BASIC_CONFIG_VERSION_FILE(): \"${CVF_COMPATIBILITY}\"")
  endif()

  if("${CVF_VERSION}" STREQUAL "")
    message(FATAL_ERROR "No VERSION specified for WRITE_BASIC_CONFIG_VERSION_FILE()")
  endif()

  configure_file("${versionTemplateFile}" "${_filename}" @ONLY)

endfunction()
