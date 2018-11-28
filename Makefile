EXEC = tp2
OBJ_FILES = algueiza.c hash.c strutil.c
C_FLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion

all:
	gcc -g $(C_FLAGS) -o $(EXEC) $(OBJ_FILES)

run:	all
	clear
	./$(EXEC)

valgrind:	all
	clear
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(EXEC)

debug:	all
	clear
	gdb -tui --args ./$(EXEC)

zip: 
	zip -r $(EXEC) utils.c heap.c heap.h abb.c abb.h hash.c hash.h strutil.c strutil.h testing.c testing.h -x "*.zip"