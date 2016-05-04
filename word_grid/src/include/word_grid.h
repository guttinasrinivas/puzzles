#ifndef __WORD_GRID_H__
#define __WORD_GRID_H__

/* Specific Constants */
#define GRID_SIDE_LEN   (4)
#define GRID_TOT_LEN    (GRID_SIDE_LEN * GRID_SIDE_LEN)

typedef struct graph_s
{
    char graph[GRID_SIDE_LEN][GRID_SIDE_LEN];
} graph_t;

typedef struct vertex_s
{
    int x;
    int y;
} vertex_t;


/* Local Symbols */
int wg_sanity_checks(char *grid, const word_list_t *wl);
int wg_word_build(char *grid, const word_list_t *wl);
int wg_check_word(char *word, graph_t *g);
int wg_str_to_grid(char *str, graph_t *g);
int wg_find_char(graph_t *g, char ch, int *x, int *y);
int wg_find_next_char(graph_t *g, char ch, int *xx, int *yy);

#endif /* __WORD_GRID_H__ */
