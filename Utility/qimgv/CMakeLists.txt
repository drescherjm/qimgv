FILE (GLOB SRC *.cpp)
list (REMOVE_ITEM SRC moc_*.cpp)
list (REMOVE_ITEM SRC *_automoc.cpp)

QT5_WRAP_UI (GUI settingsdialog.ui)

QT5_ADD_RESOURCES(RES resources.qrc)

add_executable(qimgv ${SRC} ${RES} ${GUI})

target_link_libraries(qimgv Qt5::Widgets Qt5::Concurrent overlays imagelib sourcecontainers viewers customwidgets thumbnailpanel filesystemWatchers)
     
qt5_use_modules (qimgv Core Widgets Concurrent)