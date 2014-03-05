cmake_minimum_required(VERSION 2.8)

#configure_file
#{
#	"${PROJECT_SOURCE_DIR}/"
#}

find_package(Java REQUIRED)
include(UseJava)

set(JAVA_AWT_LIBRARY /usr/lib/jvm/java-7-openjdk/jre/lib/amd64/libawt.so)
set(JAVA_JVM_LIBRARY /usr/lib/jvm/java-7-openjdk/jre/lib/amd64/server/libjvm.so)

find_package(JNI REQUIRED)
find_package(OpenCV REQUIRED)

if (JNI_FOUND)
	message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
	message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

enable_testing()

project (network_tables_com)
set(network_tables_com_VERSION_MAJOR 1)
set(network_tables_com_VERSION_MINOR 0)

# compile to class file
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.7" "-target" "1.7")
add_jar(NetworkTablesModule NetworkTablesModule.java)
get_target_property(_jarFile NetworkTablesModule JAR_FILE)
get_target_property(_classDir NetworkTablesModule CLASSDIR)

# generate .h stubs
set (_stubDir "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(
	OUTPUT NetworkTablesModule.h
	COMMAND ${Java_JAVAH_EXECUTABLE} -verbose
		-classpath ${_classDir}
		-d ${_stubDir}
		-jni NetworkTablesModule
	)

#generate libnetworktables.jnilib
include_directories(${JAVA_INCLUDE_PATH} ${_classDir} ${_stubDir})
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${_classDir})

add_executable(networktables networktables.c NetworkTablesModule.h)
target_link_libraries(networktables ${JNI_LIBRARIES})





