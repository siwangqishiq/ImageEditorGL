
cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("imageeditorgl")

add_library( # Sets the name of the library.
        imageeditorgl
        SHARED
        editor_lib.cpp
        util/shader.cpp
        util/stb_image.cpp
        util/math.hpp
        app.cpp
        widget/image.cpp
        widget/paint.cpp
        widget/origin_image.cpp
        widget/mosaic.cpp
        widget/clip.cpp
        )

find_library(opengl-lib GLESv3)
find_library(log-lib log)

target_link_libraries( # Specifies the target library.
        imageeditorgl
        android
        ${opengl-lib}
        ${log-lib}
        jnigraphics)