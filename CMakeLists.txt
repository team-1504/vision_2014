cmake_minimum_required(VERSION 2.8)

project (network_tables_com C CXX)
set(network_tables_com_VERSION_MAJOR 1)
set(network_tables_com_VERSION_MINOR 0)

#configure_file
#{
#	"${PROJECT_SOURCE_DIR}/"
#}

set(JAVA_AWT_LIBRARY /usr/lib/jvm/java-7-openjdk/jre/lib/amd64/libawt.so)
set(JAVA_JVM_LIBRARY /usr/lib/jvm/java-7-openjdk/jre/lib/amd64/server/libjvm.so)
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

include_directories(${ZLIB_INCLUDE_DIRS})

find_package(Boost 1.55 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(JNI REQUIRED)
find_package(OpenCV REQUIRED)

if (JNI_FOUND)
	message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
	message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

enable_testing()

#generate libnetworktables.jnilib
include_directories("${PROJECT_BINARY_DIR}")

#add_executable(nt_test networktables.cpp)
add_executable(pi_cam main.cpp networktables.cpp localizer.cpp identifiers.cpp constants.cpp)

#target_link_libraries(nt_test ${JNI_LIBRARIES} pthread ${Boost_LIBRARIES})
target_link_libraries(pi_cam ${JNI_LIBRARIES} pthread ${Boost_LIBRARIES} ${OpenCV_LIBRARIES})




