project(gui)

# Qt libraries
find_package(Qt5 COMPONENTS Core Gui Multimedia MultimediaWidgets Network Widgets REQUIRED)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets Qt5::Network Qt5::Widgets)

# Input
file(GLOB_RECURSE SOURCES
	"src/*.cpp"
	"src/*.ui"
	"resources/*.rc"
	"resources/*.qrc")
include_directories("src/" "../lib/src")

# Apple bundle information
if(APPLE)
	set(MACOSX_BUNDLE_ICON_FILE "organizer.icns")
	set(Organizer_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../dist/macos/resources/organizer.icns")
	set_source_files_properties(${Organizer_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	list(APPEND SOURCES ${Organizer_ICON})
endif()

# Target
if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
elseif(APPLE)
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
else()
	add_executable(${PROJECT_NAME} ${SOURCES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} lib)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Organizer")

# Install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
