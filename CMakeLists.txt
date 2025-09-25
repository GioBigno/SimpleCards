cmake_minimum_required(VERSION 3.16)

project(appsimplecards VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(simplecards
	src/main.cpp
	src/card.cpp
	src/deck.cpp
)

qt_add_qml_module(simplecards
	URI "simplecardsModule"
	VERSION 1.0
	SOURCES
		includes/deckmanager.h
		src/deckmanager.cpp
	QML_FILES
		qml/Main.qml
		qml/DecksList.qml
		qml/DecksListView.qml
		qml/TestingView.qml
		qml/Card.qml
	RESOURCES
		assets/SpaceMono-Regular.ttf
		assets/SpaceMono-Bold.ttf
		assets/SpaceMono-Italic.ttf
		assets/play.svg
		assets/stats.svg
		assets/edit.svg
		assets/back.svg
		assets/reveal.svg
		assets/correct.svg
		assets/wrong.svg
)

target_include_directories(simplecards
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(simplecards PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_target
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

target_link_libraries(simplecards
	PRIVATE 
	Qt6::Core
	Qt6::Gui
	Qt6::Qml
)

include(GNUInstallDirs)

qt_generate_deploy_qml_app_script(
    TARGET simplecards
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})

install(TARGETS simplecards
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
