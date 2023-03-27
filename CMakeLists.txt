cmake_minimum_required(VERSION 3.10)
project(test_png)
set(CMAKE_C_STANDARD 99)

# Find the libpng library
find_package(PNG REQUIRED)

# Add an executable target
add_executable(test_png pngtest.c pngwrap.c lib/stegano.c lib/stegano.h lib/message.c)
add_executable(encrypt encrypt.c pngwrap.c lib/stegano.c lib/message.c lib/neobitstream.c lib/color.c)
add_executable(decrypt decrypt.c pngwrap.c lib/stegano.c lib/message.c lib/neobitstream.c lib/color.c)
# Link the libpng library to the test_png target
target_link_libraries(test_png PRIVATE PNG::PNG) 
target_link_libraries(encrypt PRIVATE PNG::PNG m) 
target_link_libraries(decrypt PRIVATE PNG::PNG m)