cmake_minimum_required(VERSION 2.8.9)
project(service)

# default IncludeOS location #FIXME#
if (NOT DEFINED ENV{INCLUDEOS_PREFIX})
  set(ENV{INCLUDEOS_PREFIX} /usr/local)
endif()

set(SERVICE_NAME "TCP test")
set(BINARY       "test")
set(MAX_MEM 128)
set(SOURCES
    service.cpp 
  )
set(LOCAL_INCLUDES ".")

#set(LIBRARIES ${CMAKE_SOURCE_DIR}/nada.a)

# include service build script
include($ENV{INCLUDEOS_PREFIX}/includeos/service.cmake)
