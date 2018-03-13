gcc pimagens.c -c
gcc -fopenmp main.c -c
gcc -fopenmp main.o pimagens.o -o main -lm