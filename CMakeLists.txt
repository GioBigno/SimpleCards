cmake_minimum_required(VERSION 3.16)

project(simplecards VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(simplecards_target
	src/main.cpp
	src/card.cpp
	src/deck.cpp
)

#target_include_directories(flashcards_target PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

qt_add_qml_module(simplecards_target
	URI simplecards
	VERSION 1.0
	QML_FILES
		qml/Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(simplecards_target PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_target
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

target_link_libraries(simplecards_target
	PRIVATE 
	Qt6::Core
	Qt6::Gui
	Qt6::Qml
)

include(GNUInstallDirs)
install(TARGETS simplecards_target
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
