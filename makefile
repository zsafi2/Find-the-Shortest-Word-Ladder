build:
	gcc main.c -o output.txt

run:
	./output.txt

valgrind:
	valgrind ./output.txt

full_valgrind:
	valgrind --leak-check=full ./output.txt