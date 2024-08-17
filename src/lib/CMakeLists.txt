project(lib)

# Qt libraries
find_package(Qt6 COMPONENTS Core Gui Multimedia MultimediaWidgets Network Widgets REQUIRED)
set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Network Qt6::Widgets)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src/")

# Target
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

# Tests
file(GLOB_RECURSE SOURCES_TESTS "tests/*.cpp")
include_directories("src/" "../vendor/catch/src")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt6 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests ${QT_LIBRARIES} Qt6::Test lib Catch2::Catch2)
