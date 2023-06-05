echo -e "Compiling Libraries"
gcc -c Librerie/carte.c -o carte.o #
gcc -c Librerie/pilecarte.c -o pilecarte.o #
gcc -c Librerie/user.c -o user.o #
gcc -c Librerie/game.c -o game.o #
echo -e "\nCompiling main"
gcc -c menu.c -o menu.o #
echo -e "\nLinking libraries to main"
gcc -o Briscola carte.o pilecarte.o user.o game.o menu.o #
echo -e "\nDeleting object files"
rm carte.o pilecarte.o user.o game.o menu.o #
echo -e "\nDone."
