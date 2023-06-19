@echo off
cl Librerie\carte.c Librerie\pilecarte.c Librerie\user.c Librerie\game.c menu.c /link /out:Briscola.exe
echo Deleting object files
del carte.obj pilecarte.obj user.obj game.obj menu.obj 
echo Done
