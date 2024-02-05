#include "exact_cover.h"

static void print_node_state(struct Node *node) {
    printf("adress = %p, L = %p, R = %p, U = %p, D = %p\n", node, node->left, node->right, node->up, node->down);
}

static struct Column * create_header_list(int n) {
    struct Column *root = (struct Column *) calloc(n + 1, sizeof(struct Column));
    for (int i = 0; i <= n; ++i) {
        int nextIdx, prevIdx;
        nextIdx = (i + 1) % (n + 1);
        prevIdx = (i - 1) >= 0 ? (i - 1) : n;
        root[i].next = &root[nextIdx];
        root[i].prev = &root[prevIdx]; 
        root[i].id = i - 1;
    } 
    return root;
}

static struct Node ** create_nodes_matrix(int n, int m) {
    struct Node **matrix = (struct Node **) calloc(n, sizeof(struct Node *));
    for (int i = 0; i < n; ++i) {
        matrix[i] = (struct Node *) calloc(m, sizeof(struct Node));
        for (int j = 0; j < m; ++j) {
            matrix[i][j].rowId = i;
        }
    }
    return matrix;
}

static void free_nodes_matrix(struct Node **matrix, int n) {
    for (int i = 0; i < n; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

static void link_nodes(const struct array2d *initialMatrix, struct Column *root, struct Node **nodesMatrix) {
    int rows, columns;
    struct Column *columnNode;
    rows = initialMatrix->r; 
    columns = initialMatrix->c; 
    columnNode = root;
//link columns
    for (int j = 0; j < columns; ++j) {
        struct Node *prevUp;
        columnNode = columnNode->next;
        prevUp = &columnNode->head;
        for (int i = 0; i < rows; ++i) {
            if (initialMatrix->a[i][j] == 1) {
                nodesMatrix[i][j].column = columnNode;
                columnNode->size++;
                nodesMatrix[i][j].up = prevUp;
                prevUp->down = &nodesMatrix[i][j];
                prevUp = &nodesMatrix[i][j];
            }
        }
        columnNode->head.up = prevUp;
        prevUp->down = &columnNode->head;
    }

//link rows
    for (int i = 0; i < rows; ++i) {
        int first, last;
        first = last = -1; 
        for (int j = 0; j < columns; ++j) {
            if (initialMatrix->a[i][j] == 1) {
                if (first == -1) {
                    first = j;
                } else {
                    nodesMatrix[i][j].left = &nodesMatrix[i][last];
                    nodesMatrix[i][last].right = &nodesMatrix[i][j];
                }
                last = j;
            }
        }

        if (first != -1) {
            nodesMatrix[i][first].left = &nodesMatrix[i][last];
            nodesMatrix[i][last].right = &nodesMatrix[i][first];
        }
    }
}

static void cover_column(struct Column *c) {
    struct Node *r;
    c->next->prev = c->prev;
    c->prev->next = c->next;
    r = c->head.down;
    while (r != &c->head) {
        struct Node *next;
        next = r->right;
        while (next != r) {
            next->down->up = next->up;
            next->up->down = next->down;
            next->column->size--;
            next = next->right;
        }
        r = r->down;
    }
}

static void uncover_column(struct Column *c) {
    struct Node *r;
    r = c->head.up;
    while (r != &c->head) {
        struct Node *next;
        next = r->left; 
        while (next != r) {
            next->up->down = next;
            next->down->up = next;
            next->column->size++;
            next = next->left;
        }
        r = r->up;
    }
    c->next->prev = c;
    c->prev->next = c;
}

static void print_solution(const struct array *covering, int k) {
    printf("Covering rows:\n");
    for (int i = 0; i < k; ++i) {
        printf("%d ", covering->a[i]);
    }
    printf("\n");
}

static void add_solution(struct ArrayListOfArrays *list, struct array *covering, int k) {
    struct array *solution;  
    solution = (struct array *) calloc(1, sizeof(struct array));
    init_array(solution, k);
    for (int i = 0; i < k; ++i) {
        solution->a[i] = covering->a[i];
    }
    add_array_to_arraylist(list, solution);
}


static void search(struct Column *root, struct ArrayListOfArrays *list, struct array *covering, int k) {
    struct Column *nextColumn;
    struct Node *nextRow;

    if (root->next == root) {
        add_solution(list, covering, k);
        return;
    }
    nextColumn = root->next;
    cover_column(nextColumn);
    nextRow = nextColumn->head.down;
    while (nextRow != &nextColumn->head) {
        struct Node *nextNode;
        covering->a[k] = nextRow->rowId;
        nextNode = nextRow->right;
        while (nextNode != nextRow) {
            cover_column(nextNode->column);
            nextNode = nextNode->right;
        }
        search(root, list, covering, k + 1);
        nextNode = nextRow->left;
        while (nextNode != nextRow) {
            uncover_column(nextNode->column);
            nextNode = nextNode->left;
        }
        nextRow = nextRow->down;
    }
    uncover_column(nextColumn);
}

static void print_header_list(struct Column * root) {
    struct Column * nextColumn;
    nextColumn = root->next;
    printf("column %d, adress = %p, size = %d\n", root->id, root, root->size);
    while (nextColumn != root) {
        printf("column %d, size = %d\n", nextColumn->id, nextColumn->size);
        printf("adress = %p, L = %p, R = %p, U = %p, D = %p\n", nextColumn, nextColumn->prev, nextColumn->next, nextColumn->head.up, nextColumn->head.down);
        nextColumn = nextColumn->next;
    } 
}

struct ArrayListOfArrays * find_exact_cover(const struct array2d *initialMatrix) {
    int rows, columns;
    struct Column *root;
    struct Node **nodesMatrix;
    struct ArrayListOfArrays *list;
    struct array covering;
    
    if (initialMatrix == NULL) {
        printf("InitialMatrix is NULL\n");
        abort();
    }
    
    rows = initialMatrix->r;
    columns = initialMatrix->c;
    list = create_arraylist();
    init_array(&covering, rows);
    
    root = create_header_list(columns);
    nodesMatrix = create_nodes_matrix(rows, columns);
    link_nodes(initialMatrix, root, nodesMatrix);
    search(root, list, &covering, 0);
    free_nodes_matrix(nodesMatrix, rows);
    free(root);
    free_array(&covering);
    return list;
}
