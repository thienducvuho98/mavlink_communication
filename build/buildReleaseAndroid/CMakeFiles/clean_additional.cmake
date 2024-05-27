# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appCommunicationMavlink_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appCommunicationMavlink_autogen.dir\\ParseCache.txt"
  "appCommunicationMavlink_autogen"
  )
endif()
