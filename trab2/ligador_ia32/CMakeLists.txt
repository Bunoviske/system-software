project(trab2)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)
aux_source_directory(./elfio SRC_LIST1)

add_executable(ligador_ia32 ${SRC_LIST} ${SRC_LIST1})

add_definitions("-std=c++11")
