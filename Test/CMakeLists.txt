add_executable(test src/Main.cpp)

target_link_libraries(test PRIVATE
                      MKImageLib
)