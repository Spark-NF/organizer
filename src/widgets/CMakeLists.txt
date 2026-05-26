project(widgets)

# Qt libraries
find_package(Qt6 COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets REQUIRED)
set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Widgets)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.ui")
include_directories("src/" "../lib/src")

# Target
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} lib)
target_include_directories(${PROJECT_NAME} PUBLIC "src/")

# Tests
file(GLOB_RECURSE SOURCES_TESTS "tests/*.cpp")
include_directories("src/" "../tests/src/" "../vendor" "../vendor/catch/src")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt6 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests ${QT_LIBRARIES} tests Qt6::Test lib ${PROJECT_NAME} Catch2::Catch2)
target_compile_definitions(${PROJECT_NAME}-tests PRIVATE "TEST_RESOURCES=\"${CMAKE_CURRENT_SOURCE_DIR}/tests/resources\"")
