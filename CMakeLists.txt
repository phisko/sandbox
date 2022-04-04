cmake_minimum_required(VERSION 3.10)
project(sandbox)

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /std:c++20 /DNOMINMAX /MP8")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_compile_definitions(KENGINE_COMPONENT_COUNT=256)

#
# Executable
#

set(exe_name sandbox)

file(GLOB exeFiles
		src/*.cpp src/*.hpp src/*.inl)

add_executable(${exe_name} ${exeFiles})
target_include_directories(${exe_name} PRIVATE src)

# set plugin dir
set(runtime_dir $<TARGET_FILE_DIR:${exe_name}>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${runtime_dir}/plugins)

# copy resources
add_custom_command(TARGET ${exe_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
		make_directory ${runtime_dir}/resources)
add_custom_command(TARGET ${exe_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
		copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${runtime_dir}/resources)

#
# Kengine
#

set(KENGINE_TESTS ON)
set(KENGINE_ALL_SYSTEMS ON)
add_subdirectory(kengine)
#
# API
#

putils_src_files(apiFiles
		common
		common/helpers)
add_library(api STATIC ${apiFiles})
target_link_libraries(api PUBLIC kengine)
target_include_directories(api PUBLIC common)

# link to exe
target_link_libraries(${exe_name} api)

#
# Plugins
#

add_subdirectory(plugins)