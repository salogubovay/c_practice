all: disordered_square orthogonal_square

disordered_square: ../array/array.c transversals.c disordered_square_generator.c
	gcc ../array/array.c transversals.c disordered_square_generator.c -o disordered_square -O2

orthogonal_square: ../array/array.c transversals.c latin_squares.c orthogonal_square_generator.c
	gcc ../array/array.c transversals.c latin_squares.c orthogonal_square_generator.c -o orthogonal_square -O2
