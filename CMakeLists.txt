cmake_minimum_required(VERSION 3.16)

project(ProgrammingPuzzle VERSION 0.1 LANGUAGES CXX)

# FIXES FOR MACOS LINKER (AGL) AND MODERN QT
set(QT_QPA_DEFAULT_PLATFORM_PLUGIN "cocoa")
set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -Wl,-rpath,@executable_path/../Frameworks")
set(QT_NO_DEPRECATED_MOC_HEADERS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Qt SQL components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql LinguistTools)

set(TS_FILES ProgrammingPuzzle_en_IN.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    questions.cpp
    question.h
    database.h
    database.cpp
    logindialog.cpp
    logindialog.h
    logindialog.ui
    ${TS_FILES}
)

# Fix 1: Corrected conditional block syntax
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(ProgrammingPuzzle
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc  # Keep only the .qrc file here
    )

    # Add resources properly
    qt_add_resources(ProgrammingPuzzle "app_images"
        PREFIX "/images"
        FILES
        "images/Screenshot 2025-11-03 at 2.53.53 PM.png"
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ProgrammingPuzzle SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ProgrammingPuzzle
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Link Qt SQL
target_link_libraries(ProgrammingPuzzle PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Bundle info
# Fix 2: Initialize BUNDLE_ID_OPTION to prevent parsing error
set(BUNDLE_ID_OPTION "")
if(QT_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ProgrammingPuzzle)
endif()

set_target_properties(ProgrammingPuzzle PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ProgrammingPuzzle
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProgrammingPuzzle)
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,CoreFoundation")
