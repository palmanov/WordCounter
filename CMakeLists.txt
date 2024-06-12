cmake_minimum_required(VERSION 3.16)

project(WordCount VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

include_directories(include)

qt_add_executable(appWordCount
    src/main.cpp
    include/fileprocessor.h src/fileprocessor.cpp
    include/fileprocessorthread.h src/fileprocessorthread.cpp
)

qt6_add_qml_module(appWordCount
    URI WordCount
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        QML_FILES qml/WordHistogram.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appWordCount PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appWordCount
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appWordCount
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appWordCount
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
