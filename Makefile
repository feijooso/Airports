EXEC = tp2
OBJ_FILES = algueiza.c hash.c strutil.c abb.c pila.c
C_FLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
TEST_FILES = f1.csv

all:
	gcc -g $(C_FLAGS) -o $(EXEC) $(OBJ_FILES)

run:	all
	./$(EXEC)

valgrind:	all
	clear
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(EXEC)

debug:	all
	clear
	gdb -tui --args ./$(EXEC)

zip: 
	zip -r $(EXEC) algueiza.c algueiza.h utils.c heap.c heap.h abb.c abb.h hash.c hash.h strutil.c strutil.h testing.c testing.h -x "*.zip"