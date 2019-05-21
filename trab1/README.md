
##### Alunos

Bruno Fernandes Carvalho   15/0007159
Pedro Lucas Pinto Andrade  16/0038316

##### Instruções para compilar e executar usando c++11.

1- cmake .                   # configura makefile

2- make                      # compila

3- ./tradutor arquivo.asm    # executa

##### NOTA

Para COPY e chamada de MACRO, o offset de um label é feito com o número no mesmo token da vírgula:

COPY A + 1, B
MACRO A + 1, B + 1

