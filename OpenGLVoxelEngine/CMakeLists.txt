cmake_minimum_required(VERSION 3.10)
project(OpenGLVoxelEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
# These paths are relative to this CMakeLists.txt file.
# Assumes your source files are in a subdirectory called "OpenGLVoxelEngine".
set(SOURCES
    OpenGLVoxelEngine/main.cpp
    OpenGLVoxelEngine/Application.cpp
    OpenGLVoxelEngine/Renderer.cpp
    OpenGLVoxelEngine/Shader.cpp
    OpenGLVoxelEngine/glad.c
)

# Create the executable
add_executable(OpenGLVoxelEngine ${SOURCES})

# Include directories
target_include_directories(OpenGLVoxelEngine PRIVATE
    # Directory for GLAD, GLFW, GLM headers (relative to this CMakeLists.txt)
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL/Include

    # Directory for your own project's .h files (like Application.h, Shader.h)
    # This allows you to use #include "Application.h" from your .cpp files.
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVoxelEngine
)

# Link directories (for glfw3.lib or other precompiled libs)
target_link_directories(OpenGLVoxelEngine PRIVATE
    # Directory for glfw3.lib (relative to this CMakeLists.txt)
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL/Lib
)

# Link libraries
target_link_libraries(OpenGLVoxelEngine PRIVATE
    glfw3
    opengl32
    # You might need other system libraries depending on your GLFW build,
    # e.g., gdi32, user32, shell32 on Windows, but often these are found automatically.
)

# --- Section to copy shader files and other assets ---

# Define the directory where your original shader files (and other assets) are located,
# relative to this CMakeLists.txt file.
set(ASSETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVoxelEngine)

# List the asset files you want to copy.
# Add any other asset files here (e.g., textures).
set(ASSET_FILES
    ${ASSETS_SOURCE_DIR}/shader.vert
    ${ASSETS_SOURCE_DIR}/shader.frag
    # Example: ${ASSETS_SOURCE_DIR}/textures/my_texture.png
)

# For each asset file, add a custom command to copy it to the
# directory where your executable (OpenGLVoxelEngine.exe) will be built.
# This command runs after the executable is successfully built.
foreach(ASSET_FILE_PATH ${ASSET_FILES})
    get_filename_component(ASSET_FILENAME ${ASSET_FILE_PATH} NAME)
    # If your assets are in subdirectories within ASSETS_SOURCE_DIR
    # and you want to preserve that structure, you'll need to adjust the destination path.
    # For now, this copies all listed assets directly into the executable's directory.
    # If you have, for example, ASSETS_SOURCE_DIR/textures/my_texture.png,
    # and want it in <exe_dir>/textures/my_texture.png, the logic below needs enhancement.
    # For simple cases like shaders directly in ASSETS_SOURCE_DIR, this is fine.

    add_custom_command(
        TARGET OpenGLVoxelEngine POST_BUILD  # Runs after OpenGLVoxelEngine is built
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSET_FILE_PATH}"  # Source file
            "$<TARGET_FILE_DIR:OpenGLVoxelEngine>/${ASSET_FILENAME}" # Destination: executable's directory
        COMMENT "Copying asset: ${ASSET_FILENAME} to executable directory"
    )
endforeach()
# --- End of section to copy shader files and other assets ---

# Optional: If you are using Visual Studio and want to organize your source files
# in the Solution Explorer similarly to your directory structure.
# This doesn't affect compilation but helps with IDE organization.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVoxelEngine FILES ${SOURCES})