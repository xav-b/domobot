cmake_minimum_required(VERSION 2.8)
# Project Name
project(glKinectClient)

## FIND GLUT ###########################################
# sudo apt-get install libxmu-dev libxi-dev
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

## FIND OPENGL #########################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

## FIND BOOST THREAD ###################################
set(Boost_INCLUDE_DIR /usr/include/)
find_package(Boost COMPONENTS thread REQUIRED) 
include_directories(${Boost_INCLUDE_DIR})
if(NOT Boost_FOUND)
    message(ERROR " Boost not found!")
endif(NOT Boost_FOUND)

#######################################################
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -Wall")
endif()

## Include Files #######################################
set(SOURCES glKinectClient.cpp)
message(STATUS "sources: " ${SOURCES})
add_executable(glKinectClient ${SOURCES})

## Linking & stuff ######################################

# create the program "glKinectClient"
target_link_libraries(glKinectClient ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${Boost_LIBRARIES})

#add_custom_command(TARGET glKinectClient
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy glKinectClient ../)
