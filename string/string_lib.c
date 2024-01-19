#include "string_lib.h"

static void check_stream(FILE *stream) {
    if (stream == NULL) {
        printf("stream is NULL\n");
        abort;
    }
}

char * read_full_line(FILE *stream) {
    char *line;
    check_stream(stream); 
    line = (char *) calloc(BUFFER_SIZE, sizeof(char));
    if (fgets(line, BUFFER_SIZE, stream) != NULL) {
        return line;
    } else {
        printf("Fail to read from input stream\n");
        abort();
    }
}

char * read_line(FILE *stream) {
    char *line;
    check_stream(stream); 
    line = read_full_line(stream);
    line[strcspn(line, "\n")] = '\0';
    return line;
}

char * generate_empty_string() {
    char *str = (char *) calloc(1, sizeof(char));
    str[0] = '\0';
    return str;
}

void append_string(char **pDest, const char *src) {
    size_t size;
    if (pDest == NULL) {
        printf("Destination string is NULL\n");
        abort();
    }
    size = strlen(*pDest) + strlen(src) + 1;
    *pDest = (char *) realloc(*pDest, size);
    strcat(*pDest, src);
}

void check_file(const char *fileName) {
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Unable to open file '%s'\n", fileName);
        abort();
    }
    fclose(file);
}

void read_input_to_str(char **pstr, FILE *stream) {  
    char *line;
    check_stream(stream);
    line = (char *) calloc(BUFFER_SIZE, sizeof(char));
    while (fgets(line, BUFFER_SIZE, stream) != NULL) {
        append_string(pstr, line);
    }
    free(line);
}
