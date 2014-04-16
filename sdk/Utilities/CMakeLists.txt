cmake_minimum_required(VERSION 2.8.9)
if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif()

project(Utilities)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

set(Utilities_Sources
#	HttpRequestSync.cpp
	utilitiesMath.cpp
)

set(Utilities_Headers
	HttpRequestSync.h
	utilitiesMath.h
	utilities_define.h
)

set(Utilities_Libraries
	${Boost_LIBRARIES}
)

include_directories(${SDK_INCLUDES})
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_library(Utilities SHARED
	${Utilities_Sources}
	${Utilities_Headers}
)

target_link_libraries(Utilities ${Utilities_Libraries})
qt5_use_modules(Utilities Core Network)

set_target_properties(Utilities PROPERTIES 
	FOLDER "sdk"
)

create_target_launcher(Utilities
	RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
