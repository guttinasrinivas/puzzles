#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
#include "queue_impl.h"
#include "word_list.h"

/* Algorithm specific symbols. */
int sanity_checks(char *start, char *end, const word_list_t *wl);
int word_traverse(char *start, char *end, const word_list_t *wl, int *len);

static void usage(char *pname)
{
    printf("%s <start word> <end word> <dictionary file>\n", pname);
    return;
}

int main(int argc, char **argv)
{
    int ret = 0;
    char *start = NULL;
    char *end = NULL;
    char *dict_name = NULL;
    int len = 0;
    word_list_t wl;
    
    /* Simple args check. 
     * TODO Better checks. */
	if (argc != 4)
    {
        usage(argv[0]);
        return(EINVAL);
    }
    else
    {
        start = argv[1];
        end = argv[2];
        dict_name = argv[3];
    }
    
    wl.fname = dict_name;
    ret = wl_read_list(&wl);
    RET_MSG_ON_ERR(ret, "Dictionary file access error.\n");
    
    LOG_PRINTF(INFO, "Starting from \"%s\" to \"%s\" through \"%s\"...\n", start, end, argv[3]);
    ret = word_traverse(start, end, &wl, &len);
    
	return(ret);
}

int word_traverse(char *start, char *end, const word_list_t *wl, int *len)
{
    int ret = 0;
    int pos = 0;
    int ii = 0;
    char ch = 'a';
    char *t_w = NULL;
    char *curr_w = NULL;
    
    q_t word_q;
    q_node_t *n;
    q_node_t *tmp_n;
    
    ret = sanity_checks(start, end, wl);
    RET_ON_ERR(ret);
    
    /* Start with queue */
    ret = q_init(&word_q);
    RET_ON_ERR(ret);
    
    curr_w = strndup(start, strlen(start));
    ret = q_init_node(&n, (void*) curr_w);
    RET_ON_ERR(ret);
    
    t_w = strndup(start, strlen(start));
    while (strncasecmp(curr_w, end, strlen(curr_w)) != 0)
    {
        /*
         * Algorithm: For all possible letter combinations, find
         * all possible valid words from the dictionary. For all
         * possible words from the list, perform an exhaustive
         * BFS until the required word is found.
        */
        for (ii = 0; ii < strlen(start); ii++)
        {
            strncpy(t_w, curr_w, strlen(start));
            for (ch = 'a'; ch <= 'z'; ch++)
            {
                t_w[ii] = ch;
                if (wl_lookup(wl, t_w, &pos) != SUCCESS)
                {
                    continue;
                }
                ret = q_init_node(&tmp_n, (void*) strndup(t_w, strlen(start)));
                RET_ON_ERR(ret);
                ret = q_fifo_in(&word_q, tmp_n);
                RET_ON_ERR(ret);
            }
        }
        
        ret = q_free_node(&n);
        RET_ON_ERR(ret);
        
        ret = q_fifo_out(&word_q, &n);
        RET_ON_ERR(ret);
        
        curr_w = (char*) n->data;
    }
    
    return(ret);
}

int sanity_checks(char *start, char *end, const word_list_t *wl)
{
    int ii = 0;
    int pos = 0;
    int ret = SUCCESS;
    
    /* Preliminary checks */
    if ((start == NULL) ||
        (end == NULL))
    {
        EPRINTF("Invalid inputs to word_traverse\n");
        return(EINVAL);
    }
    
    if (strlen(start) != strlen(end))
    {
        LOG_PRINTF(ERROR, "This implemenation only supports words of same size.\n");
        return(EINVAL);
    }
    
    for (ii = 0; ii < strlen(start); ii++)
    {
        start[ii] = tolower(start[ii]);
    }
    
    for (ii = 0; ii < strlen(end); ii++)
    {
        end[ii] = tolower(end[ii]);
    }
    
    if ((wl_lookup(wl, start, &pos) != 0) ||
        (wl_lookup(wl, end, &pos) != 0))
    {
        LOG_PRINTF(ERROR, "Given word(s) are not in the dictionary.\n");
        return(ENOTSUP);
    }
    
    return(ret);
}
