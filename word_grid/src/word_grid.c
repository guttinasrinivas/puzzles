#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
#include "word_list.h"
#include "word_grid.h"

static const vertex_t v_fst[] = { 
        { -1, -1 },
        { -1, 0 },
        { -1, 1 },
        { 0, -1 },
        { 0, 1 },
        { 1, -1 },
        { 1, -1 },
        { 1, 0 },
        { 1, 1 }
    };

int wg_word_build(char *grid, const word_list_t *wl)
{
    int ret = 0;
    int ii = 0;
    graph_t g;
    
    /* Sanity checks & everything to lower case */
    ret = wg_sanity_checks(grid, wl);
    RET_ON_ERR(ret);
    
    ret = wg_str_to_grid(grid, &g);
    RET_ON_ERR(ret);
    
    /* Algorithm:
     * Build all possible valid words after removing redundant entries from
     * the input grid list, by checking the dictionary. From the available
     * word list, check to see if a word could be found as adjacent vertices
     * or interconnected edges of an undirected graph. */
    
    /* Start with first word */
    for (ii = 0; ii < wl->n_words; ii++)
    {
        ret = wg_check_word(wl->words[ii], &g);
        if (ret == SUCCESS)
        {
            printf("%s\n", wl->words[ii]);
        }
    }
    
    return(SUCCESS);
}

int wg_check_word(char *word, graph_t *g)
{
    int ii = 0;
    int xx = 0;
    int yy = 0;
    char ch = word[0];
    graph_t *t_g = NULL;
    int ret = SUCCESS;
    
    t_g = malloc(sizeof(graph_t));
    memcpy((void*) t_g, (void*) g, sizeof(graph_t));
    
    int w_len = strlen(word);
    
    DBG_PRINTF("Word: %s\n", word);
        
    ret = wg_find_char(t_g, ch, &xx, &yy);
    RET_ON_ERR(ret);
    
    DBG_PRINTF("Found first char %c at xx: %d, yy: %d.\n",
               ch, xx, yy);
    
    for (ii = 1; ii < w_len; ii++)
    {
        ret = wg_find_next_char(t_g, word[ii], &xx, &yy);
        RET_ON_ERR(ret);
    }
    
    return(SUCCESS);
}

int wg_find_char(graph_t *g, char ch, int *x, int *y)
{
    int xx = 0;
    int yy = 0;
    
    for (xx = 0; xx < GRID_SIDE_LEN; xx++)
    {
        for (yy = 0; yy < GRID_SIDE_LEN; yy++)
        {
            if (g->graph[xx][yy] == ch)
            {
                g->graph[xx][yy] = '\0';
                *x = xx;
                *y = yy;
                return(SUCCESS);
            }
        }
    }
    
    return(E_NOTFOUND);
}

int wg_find_next_char(graph_t *g, char ch, int *xx, int *yy)
{
    int ii = 0;
    int rd = 0;
    int cd = 0;
    
    for (ii = 0; ii < 8; ii++)
    {
        rd = *xx + v_fst[ii].x;
        cd = *yy + v_fst[ii].y;

        if ((rd >= GRID_SIDE_LEN) ||
            (rd < 0) ||
            (cd >= GRID_SIDE_LEN) ||
            (cd < 0))
        {
            continue;
        }

        if (g->graph[rd][cd] != ch)
        {
            continue;
        }

        g->graph[rd][cd] = '\0';
        *xx = rd;
        *yy = cd;
        return(SUCCESS);
    }

    return(E_NOTFOUND);
}

int wg_sanity_checks(char *grid, const word_list_t *wl)
{
    int ii = 0;
    int ret = SUCCESS;
    
    /* Preliminary checks */
    if (grid == NULL)
    {
        EPRINTF("Invalid inputs to word_traverse\n");
        return(E_ARGS);
    }
    
    if (strlen(grid) != GRID_TOT_LEN)
    {
        EPRINTF("Input must be %d characters to form a %dx%d grid.\n",
                GRID_TOT_LEN, GRID_SIDE_LEN, GRID_SIDE_LEN);
        return(E_ARGS);
    }
    
    for (ii = 0; ii < strlen(grid); ii++)
    {
        grid[ii] = tolower(grid[ii]);
    }
    
    return(ret);
}

/* Utility */
int wg_str_to_grid(char *str, graph_t *g)
{
    int ret = SUCCESS;
    int ii = 0;
    int xx = 0;
    int yy = 0;
    
    for (ii = 0; ii < strlen(str); ii++)
    {
        g->graph[xx][yy] = str[ii];
        xx++;
        if (xx >= GRID_SIDE_LEN)
        {
            xx = 0;
            yy++;
        }
    }
    
    return(ret);
}
