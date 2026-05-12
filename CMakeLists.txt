cmake_minimum_required(VERSION 3.16)

project(DicomViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

find_package(DCMTK REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDicomViewer
    main.cpp
)

qt_add_qml_module(appDicomViewer
    URI DicomViewer
    QML_FILES
        Main.qml
    SOURCES
        backend/dicomloader.h
        backend/dicomloader.cpp
        backend/dicomimageprovider.h
        backend/dicomimageprovider.cpp
        backend/dicomcontroller.h
        backend/dicomcontroller.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDicomViewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDicomViewer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDicomViewer
    PRIVATE Qt6::Quick DCMTK::DCMTK
)

include(GNUInstallDirs)
install(TARGETS appDicomViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
