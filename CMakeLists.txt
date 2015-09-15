cmake_minimum_required (VERSION 2.8.11)
project (Bourso)

add_executable (bourso bourso_parser.c downloader.c)

target_link_libraries(bourso curl)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTEST_RUC")

add_executable (download downloader.c)

target_link_libraries(download curl)