file(GLOB SOURCES *.cpp)
list (REMOVE_ITEM SOURCES moc_*.cpp)
list (REMOVE_ITEM SOURCES *_automoc.cpp)
add_library(filesystemWatchers STATIC ${SOURCES})
target_link_libraries(filesystemWatchers Qt5::Widgets)
