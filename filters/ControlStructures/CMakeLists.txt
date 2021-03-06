cmake_minimum_required(VERSION 2.8.9)
if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif()

project(ControlStructures)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

set(ControlStructures_Sources
	branch.cpp
	join.cpp
)

set(ControlStructures_Headers
	branch.h
	join.h
)

set(ControlStructures_Libraries
	${Boost_LIBRARIES}
	Utilities.lib
	Base.lib
	Media.lib
)

include_directories(${SDK_INCLUDES})
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_library(ControlStructures SHARED
	${ControlStructures_Sources}
	${ControlStructures_Headers}
)

target_link_libraries(ControlStructures ${ControlStructures_Libraries})

set_target_properties(ControlStructures PROPERTIES 
	FOLDER "filters"
)

add_dependencies(ControlStructures Utilities)
add_dependencies(ControlStructures Base)
add_dependencies(ControlStructures Media)

create_target_launcher(ControlStructures
	RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
