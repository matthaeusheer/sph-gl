# Projects Settings
cmake_minimum_required(VERSION 3.1)
project(opengl-skeleton)

# The main executable
add_executable(main
        src/main.cpp
        #src/utils/datamanager.cpp
        src/graphics/camera.cpp
        src/graphics/shader.cpp
        src/graphics/window.cpp
        src/graphics/buffers/buffer.cpp
        src/graphics/buffers/indexbuffer.cpp
        src/graphics/buffers/vertexarray.cpp
        Dependencies/GLAD/glad.c
        )

target_include_directories(main PRIVATE
        src/utils
        src/graphics
        src/graphics/buffers
        Dependencies/GLAD/include
        Dependencies/StdImage/include
)

set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_compile_options(main PRIVATE -Wall)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)
