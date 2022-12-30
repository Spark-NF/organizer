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

# Tests
file(GLOB_RECURSE SOURCES_TESTS "tests/*.cpp")
include_directories("src/" "..")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt5 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests ${QT_LIBRARIES} Qt5::Test lib)
