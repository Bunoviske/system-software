project(trab2)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)
aux_source_directory(./translator/interfaces SRC_LIST1)
aux_source_directory(./utils SRC_LIST2)
aux_source_directory(./translator/translate SRC_LIST3)
aux_source_directory(./translator/baseClass SRC_LIST4)

add_executable(tradutor ${SRC_LIST} ${SRC_LIST1} ${SRC_LIST2}
               ${SRC_LIST3} ${SRC_LIST4})

add_definitions("-std=c++11")
