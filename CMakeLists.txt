cmake_minimum_required(VERSION 3.0.0)
project(MusicServer VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("./inc" "/usr/local/include")

find_library(NAMES sndfile PATHS /usr/local/lib)

link_libraries(ao sndfile pthread dl mpg123)
add_executable(MusicServer main.c src/cmd.c src/server.c src/server_commands.c src/music.c src/files.c src/global_cfg.c src/sqlite3.c src/db.c)
add_compile_options(-g -wall -lpthread -ldl -lmpg123 -DENABLE_MPEG=ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
