set(lib_src
    src/ImageFile.cpp
    src/FileType.cpp
    src/Image.cpp
    src/Interpolation.cpp
    src/Globals.cpp
)

add_library(MKImageLib ${lib_src})
set_target_properties(MKImageLib PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      CXX_EXTENSIONS OFF
)
target_compile_options(MKImageLib PUBLIC
                       -std=c++17
)
target_link_libraries(MKImageLib PRIVATE
                      -lstdc++fs
                      -pthread
)

target_include_directories(MKImageLib PUBLIC
                           src
)