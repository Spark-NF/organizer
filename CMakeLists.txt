cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(ignoreMe "${QT_QMAKE_EXECUTABLE}") # Ignore "Manually-specified variables were not used by the project" warnings

# Use C++ 11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable more compiler warnings
if(MSVC OR CMAKE_CXX_COMPILER MATCHES "cl\\.exe")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4714 /wd4127") # Suppress some warnings
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

# Build information
string(REGEX REPLACE "^v" "" VERSION "${VERSION}")
if(CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64")
	set(VERSION_PLATFORM "x64")
else()
	set(VERSION_PLATFORM "x86")
endif()

# Project settings
project(Organizer)
add_definitions(-DVERSION="${VERSION}")
add_definitions(-DVERSION_PLATFORM="${VERSION_PLATFORM}")
add_definitions(-DPROJECT_GITHUB_URL="https://github.com/Spark-NF/organizer")

# Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets Qt5::Network Qt5::Widgets)

# Input
file(GLOB_RECURSE SOURCES
	"src/*.cpp"
	"src/*.ui"
	"resources/*.rc"
	"resources/*.qrc")
include_directories("src/")

# Apple bundle information
if(APPLE)
	set(MACOSX_BUNDLE_ICON_FILE "organizer.icns")
	set(Organizer_ICON "${CMAKE_CURRENT_SOURCE_DIR}/dist/macos/resources/organizer.icns")
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
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

# Install
if(UNIX AND (NOT APPLE))
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
	install(FILES "dist/linux/organizer.desktop" DESTINATION "share/applications")
	install(FILES "dist/linux/hicolor/48x48/organizer.png" DESTINATION "share/icons/hicolor/48x48/apps")
	install(FILES "dist/linux/hicolor/96x96/organizer.png" DESTINATION "share/icons/hicolor/96x96/apps")
	install(FILES "dist/linux/hicolor/128x128/organizer.png" DESTINATION "share/icons/hicolor/128x128/apps")
	install(FILES "dist/linux/organizer.appdata.xml" DESTINATION "share/metainfo")
endif()
