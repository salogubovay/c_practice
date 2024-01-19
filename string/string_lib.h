#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 200

char * read_full_line(FILE *stream);
char * read_line(FILE *stream);
char * generate_empty_string();
void append_string(char **pDest, const char *src);
void check_file(const char *fileName);
void read_input_to_str(char **pstr, FILE *stream);
