gcc -c parser/lexer.c -o parser/lexer.o
gcc -c lib/logging.c -o lib/logging.o
gcc -c lib/svwm.c -o lib/svwm.o
gcc -c vendor/sqlite3.c -o vendor/sqlite3.o
g++ -c text_functions.cpp -o text_functions.o
g++ -c TuringMachine.cpp -o TuringMachine.o
g++ -c main.cpp -o main.o
g++ -o main  main.o parser/lexer.o lib/logging.o lib/svwm.o vendor/sqlite3.o TuringMachine.o text_functions.o -ldl -lpthread
