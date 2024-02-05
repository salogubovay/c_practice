all: exact_cover orthogonal_generator

exact_cover: ../array/array.c ../lists/lists.c exact_cover.c exact_cover_search.c
	gcc ../array/array.c ../lists/lists.c exact_cover.c -o exact_cover exact_cover_search.c -O2

orthogonal_generator: ../array/array.c \
                 ../graeco_latin_squares/latin_squares.c \
                 ../graeco_latin_squares/transversals.c \
                 ../lists/lists.c exact_cover.c \
                   incidence_matrix.c \
                   orthogonal_generator.c
	gcc ../array/array.c \
        ../graeco_latin_squares/latin_squares.c \
        ../graeco_latin_squares/transversals.c \
        ../lists/lists.c \
        exact_cover.c \
        incidence_matrix.c \
        orthogonal_generator.c -o orthogonal_generator -O2
