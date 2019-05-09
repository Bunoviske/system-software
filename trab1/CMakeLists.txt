project(assembler)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)
aux_source_directory(./assembler/interfaces SRC_LIST1)
aux_source_directory(./utils SRC_LIST2)
aux_source_directory(./assembler/preproc SRC_LIST3)
aux_source_directory(./assembler/passage1 SRC_LIST4)
aux_source_directory(./assembler/passage2 SRC_LIST5)
aux_source_directory(./assembler/errorAnalysis SRC_LIST6)
aux_source_directory(./assembler/baseClasses SRC_LIST7)

add_executable(tradutor ${SRC_LIST} ${SRC_LIST1} ${SRC_LIST2} 
               ${SRC_LIST3} ${SRC_LIST4} ${SRC_LIST5} ${SRC_LIST6} ${SRC_LIST7})
               
add_definitions("-std=c++11")
