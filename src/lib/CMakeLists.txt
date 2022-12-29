project(lib)

# Qt libraries
find_package(Qt5 COMPONENTS Core Gui Multimedia MultimediaWidgets Network Widgets REQUIRED)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets Qt5::Network Qt5::Widgets)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src/")

# Target
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
