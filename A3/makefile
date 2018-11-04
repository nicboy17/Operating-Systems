OPTIONS = -std=c11 -Wall -Iinclude

all: src/dine.c src/holes.c
	gcc $(OPTIONS) -pthread src/dine.c -o dine
	gcc $(OPTIONS) src/holes.c -o holes

dine: src/dine.c
	gcc $(OPTIONS) -pthread src/dine.c -o dine

holes: src/holes.c
	gcc $(OPTIONS) src/holes.c -o holes

clean:
	rm dine holes
