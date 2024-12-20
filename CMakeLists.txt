cmake_minimum_required(VERSION 3.9)
project(game_engine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(src/shapes)
include_directories(tinyobjloader)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Правильное подключение компонентов Qt6
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(assimp REQUIRED)

set(WINDOWS_SRC
    src/main.cpp
    src/app.cpp
    src/opengl_win.cpp
    src/camera.cpp
    src/physics.cpp
)

set(SHAPES_SRC
    src/shapes/cube.cpp
    src/shapes/shape.cpp
    src/shapes/mesh.cpp
    src/shapes/obj_loader.cpp
    src/shapes/light.cpp
    src/shapes/aabb.cpp
)

set(RESOURCES
    src/resources/resourses.qrc
)

qt_add_resources(RESOURCES_GENERATED ${RESOURCES})

add_library(shapes STATIC ${SHAPES_SRC})

target_link_libraries(shapes Qt6::Core Qt6::OpenGL assimp::assimp)

qt6_wrap_ui(UI_HEADERS src/mainwindow.ui)

add_executable(app
    ${WINDOWS_SRC}
    ${UI_HEADERS}
)

target_link_libraries(app Qt6::Widgets Qt6::OpenGLWidgets shapes)
target_sources(app PRIVATE ${RESOURCES_GENERATED})