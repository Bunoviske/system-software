
##### Alunos

Bruno Fernandes Carvalho   15/0007159
Pedro Lucas Pinto Andrade  16/0038316

##### Notas importantes

- Deve-se garantir que o executável "tradutor" esteja na mesma pasta do arquivo main, pois a partir dessa pasta é feita a leitura do arquivo que tem as funções implementadas em assembly

- Deve-se garantir que o executável "ligador_ia32" esteja na mesma pasta do arquivo main, pois a partir dessa pasta é feita a leitura do arquivo que tem o código máquina das funções implementadas em assembly

- Se compilar conforme as instruções abaixo, tudo dá certo

##### Instruções para compilar e executar o TRADUTOR usando c++11.

1- cmake .                   # configura makefile

2- make                      # compila

3- ./tradutor arquivo.asm    # executa o tradutor

##### Instruções para compilar e executar o LIGADOR_IA32 usando c++11.

1- cmake .                   # configura makefile

2- make                      # compila

3- ./ligador_ia32 arquivo.s    # executa o tradutor



