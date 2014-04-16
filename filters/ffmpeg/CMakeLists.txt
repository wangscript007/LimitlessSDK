cmake_minimum_required(VERSION 2.8.9)
if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif()

project(ffmpeg)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

set(ffmpeg_Sources
	codecDescription.cpp
	ffmpegcontrols.cpp
	ffmpegDecoder.cpp
	ffmpegEncoder.cpp
	ffmpegInput.cpp
	ffmpegMediaFilter.cpp
	ffmpegOutput.cpp
	ffmpegPacketSample.cpp
	ffmpegResources.cpp
	formatDescription.cpp
)

set(ffmpeg_Headers
	codecDescription.h
	ffmpegcontrols.h
	ffmpegDecoder.h
	ffmpegEncoder.h
	ffmpegInput.h
	ffmpegMediaFilter.h
	ffmpegOutput.h
	ffmpegPacketSample.h
	ffmpegResources.h
	formatDescription.h
)

set(ffmpeg_Libraries
	${Boost_LIBRARIES}
	Utilities.lib
	Base.lib
	Media.lib
	AvStream.lib
#	optimized DirectShowBases.lib
#	debug DirectShowBasesd.lib
#	optimized ${CMAKE_SOURCE_DIR}/libraries/DirectShow/baseclasses/release/strmbase.lib
#	debug ${CMAKE_SOURCE_DIR}/libraries/DirectShow/baseclasses/debug/strmbasd.lib
	avcodec.lib
	avformat.lib
	avutil.lib
	swscale.lib
)

QT5_WRAP_UI(ffmpeg_Uis
    ffmpegcontrols.ui
)

include_directories(${SDK_INCLUDES})
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${FFMPEG_INCLUDES})
link_directories(${FFMPEG_LIB_PATH})

add_library(ffmpeg SHARED
	${ffmpeg_Sources}
	${ffmpeg_Headers}
	${ffmpeg_Uis}
)

target_link_libraries(ffmpeg ${ffmpeg_Libraries})
qt5_use_modules(ffmpeg Core Gui Widgets)

set_target_properties(ffmpeg PROPERTIES 
	FOLDER "filters"
)

add_dependencies(ffmpeg Utilities)
add_dependencies(ffmpeg Base)
add_dependencies(ffmpeg Media)
add_dependencies(ffmpeg AvStream)

create_target_launcher(ffmpeg
	RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
