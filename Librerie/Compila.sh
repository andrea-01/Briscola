cp ../menu.c .
gcc -c carte.c -o carte.o
gcc -c pilecarte.c -o pilecarte.o
gcc -c user.c -o user.o
gcc -c game.c -o game.o
gcc -c menu.c -o menu.o
gcc -o menu carte.o pilecarte.o user.o game.o menu.o
mv menu ..
rm carte.o pilecarte.o user.o game.o menu.o menu.c
