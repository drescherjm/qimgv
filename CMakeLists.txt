cmake_minimum_required(VERSION 2.8)

project(qimgv)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

include_directories(res ${Qt5Widgets_INCLUDES} ${Qt5Concurrent_INCLUDES} ${Qt5Multimedia_INCLUDES} ${Qt5MultimediaWidgets_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Concurrent_DEFINITIONS} ${Qt5Multimedia_DEFINITIONS} ${Qt5MultimediaWidgets_DEFINITIONS})

if( NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Concurrent_EXECUTABLE_COMPILE_FLAGS} ${Qt5Multimedia_EXECUTABLE_COMPILE_FLAGS} ${Qt5MultimediaWidgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")
endif(NOT MSVC)

#########################################################################################

macro(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
endmacro()

#########################################################################################

#add_subdirectory(tests)
add_subdirectory(overlays)
add_subdirectory(lib)
add_subdirectory(thumbnailPanel)
add_subdirectory(sourceContainers)
add_subdirectory(viewers)
add_subdirectory(customWidgets)
add_subdirectory(filesystemWatchers)
add_subdirectory(Utility)

# binary
install(PROGRAMS qimgv DESTINATION bin/)
# .desktop
install(FILES distrib/qimgv.desktop 
	DESTINATION share/applications)
# icons
install(FILES distrib/hicolor/16x16/apps/qimgv.png
	DESTINATION share/icons/hicolor/16x16/apps)
install(FILES distrib/hicolor/22x22/apps/qimgv.png
	DESTINATION share/icons/hicolor/22x22/apps)
install(FILES distrib/hicolor/24x24/apps/qimgv.png
	DESTINATION share/icons/hicolor/24x24/apps)
install(FILES distrib/hicolor/32x32/apps/qimgv.png
	DESTINATION share/icons/hicolor/32x32/apps)
install(FILES distrib/hicolor/48x48/apps/qimgv.png
	DESTINATION share/icons/hicolor/48x48/apps)
install(FILES distrib/hicolor/64x64/apps/qimgv.png
	DESTINATION share/icons/hicolor/64x64/apps)
install(FILES distrib/hicolor/128x128/apps/qimgv.png
	DESTINATION share/icons/hicolor/128x128/apps)
install(FILES distrib/hicolor/256x256/apps/qimgv.png
	DESTINATION share/icons/hicolor/256x256/apps)

