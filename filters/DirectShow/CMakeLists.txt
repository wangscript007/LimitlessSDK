cmake_minimum_required(VERSION 2.8.9)
if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif()

project(DirectShow)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

set(DirectShow_Sources
	DirectShowCapture.cpp
)

set(DirectShow_Headers
	DirectShowCapture.h
)

set(DirectShow_Libraries
	${Boost_LIBRARIES}
	Utilities.lib
	Base.lib
	Media.lib
	AvStream.lib
#	optimized DirectShowBases.lib
#	debug DirectShowBasesd.lib
	optimized ${CMAKE_SOURCE_DIR}/libraries/DirectShow/baseclasses/release/strmbase.lib
	debug ${CMAKE_SOURCE_DIR}/libraries/DirectShow/baseclasses/debug/strmbasd.lib
)

include_directories(${SDK_INCLUDES})
link_directories(${LIBRARY_OUTPUT_PATH})

#include_directories(${CMAKE_SOURCE_DIR}/libraries/DirectShow)
#link_directories(${CMAKE_SOURCE_DIR}/libraries/DirectShowBases/lib)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_library(DirectShow SHARED
	${DirectShow_Sources}
	${DirectShow_Headers}
)

target_link_libraries(DirectShow ${DirectShow_Libraries})
qt5_use_modules(DirectShow Core Widgets)

set_target_properties(DirectShow PROPERTIES 
	FOLDER "filters"
)

add_dependencies(DirectShow Utilities)
add_dependencies(DirectShow Base)
add_dependencies(DirectShow Media)
add_dependencies(DirectShow AvStream)

create_target_launcher(DirectShow
	RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
