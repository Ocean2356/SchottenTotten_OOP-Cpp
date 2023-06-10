cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
project("SchottenTotten")

aux_source_directory(./cpp cpp_srcs)
aux_source_directory(./h h_srcs)

add_executable(main ${cpp_srcs} ${h_srcs})
