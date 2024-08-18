project(gui)

# Qt libraries
find_package(Qt6 COMPONENTS Core Gui Multimedia MultimediaWidgets Network Widgets REQUIRED)
set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Network Qt6::Widgets)

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
if(UNIX AND (NOT APPLE))
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif()

# Tests
file(GLOB_RECURSE SOURCES_TESTS "tests/*.cpp")
list(APPEND SOURCES_TESTS ${SOURCES})
list(REMOVE_ITEM SOURCES_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
include_directories("src/" "../vendor" "../vendor/catch/src")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt6 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests ${QT_LIBRARIES} Qt6::Test lib Catch2::Catch2)
