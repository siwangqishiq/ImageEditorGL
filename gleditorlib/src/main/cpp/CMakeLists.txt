
cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("imageeditorgl")

add_library( # Sets the name of the library.
        imageeditorgl
        SHARED
        editor_lib.cpp
        shader.cpp
        stb_image.cpp
        math.hpp
        app.cpp
        image.cpp
        paint.cpp
        origin_image.cpp
        mosaic.cpp
        clip.cpp
        )

find_library(opengl-lib GLESv3)
find_library(log-lib log)

target_link_libraries( # Specifies the target library.
        imageeditorgl
        android
        ${opengl-lib}
        ${log-lib}
        jnigraphics)