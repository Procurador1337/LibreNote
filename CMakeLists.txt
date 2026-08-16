cmake_minimum_required(VERSION 3.16)

project(LibreNote VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    shortcutmanager.h
    shortcutmanager.cpp
    editorsettings.h
    editorsettings.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LibreNote
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        appsettingspath.h appsettingspath.cpp
    )
else()
    if(ANDROID)
        add_library(LibreNote SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(LibreNote
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LibreNote PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)


if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LibreNote
    )
endif()

set_target_properties(LibreNote PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS LibreNote
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES icons/librenote.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps
    RENAME librenote.png
)

install(FILES LibreNote.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LibreNote)
endif()
