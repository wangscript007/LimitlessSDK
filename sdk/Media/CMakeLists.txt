cmake_minimum_required(VERSION 2.8.9)
if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif()

project(Media)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

set(Media_Sources
	BufferSample.cpp
	GPUBufferSample.cpp
	GPUBufferGLSample.cpp
	GPUContext.cpp
	GPUImageSample.cpp
	GPUImageGLSample.cpp
	IFileSource.cpp
	IImageSample.cpp
	IImageSource.cpp
	ImageSample.cpp
	ImageSetSample.cpp
	IMediaFilter.cpp
	IMediaSource.cpp
	MediaClock.cpp
	MediaFilterContainer.cpp
	MediaFormat.cpp
	MediaPad.cpp
	MediaPluginFactory.cpp
	MediaSample.cpp
	MediaSampleFactory.cpp
	MediaSinkPad.cpp
	MediaSourcePad.cpp
	SystemMediaClock.cpp
)

set(Media_Headers
	BufferSample.h
	GPUBufferSample.h
	GPUBufferGLSample.h
	GPUContext.h
	GPUImageSample.h
	GPUImageGLSample.h
	IFileSource.h
	IImageSample.h
	IImageSetSample.h
	IImageSource.h
	ImageSample.h
	ImageSetSample.h
	IMediaFilter.h
	IMediaSource.h
	MediaClock.h
	MediaContainer.h
	MediaFilterContainer.h
	MediaFormat.h
	MediaPad.h
	MediaPluginFactory.h
	MediaSample.h
	MediaSampleFactory.h
	MediaSinkPad.h
	MediaSourcePad.h
	media_define.h
	SystemMediaClock.h
)

set(Media_Libraries
	${Boost_LIBRARIES}
	${OPENCL_LIBRARIES}
	Utilities.lib
	Base.lib
	opengl32.lib
	glew64.lib
)

include_directories(${SDK_INCLUDES})
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${OPENCL_INCLUDE_DIRS})
link_directories(${OPENCL_LIB_DIR})

add_library(Media SHARED
	${Media_Sources}
	${Media_Headers}
	${Media_Qt_Headers_Moc}
	${Media_Qt_Forms_Headers} 
    ${Media_Qt_Resources_Rcc}
)

target_link_libraries(Media ${Media_Libraries})
qt5_use_modules(Media Core)

#disable dll export warnings
add_definitions("/wd4251")

set_target_properties(Media PROPERTIES 
	FOLDER "sdk"
)

add_dependencies(Media Base)
add_dependencies(Media Utilities)

create_target_launcher(Media
	RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
