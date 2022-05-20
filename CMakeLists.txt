cmake_minimum_required(VERSION 3.0.0)
project(MusicServer VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("./inc")

add_executable(MusicServer main.c src/cmd.c src/server.c src/server_commands.c)
add_compile_options(-g)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
