#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
#include "queue_impl.h"
#include "word_list.h"


#define CleanUp __attribute__((cleanup (mem_free)))

int LOG_CurrLevel = DEBUG;
static char *ENV_EnQueuePrint = NULL;

/* Algorithm specific symbols. */
static int sanity_checks(char *start, char *end, word_list_t *wl, uint8_t *valid_bm);
static int word_traverse(char *start, char *end, word_list_t *wl, int *len);
static void debug_print_q(q_t *word_q);


static void mem_free(uint8_t **buf)
{
    free(*buf);
}


static void ref_ct_dwn(RefCtdObj_t **bufp)
{
    AssertMarker(*bufp);

    LOG_Printf(DEBUG, "Ref count down: (0x%llx)", (unsigned long long) *bufp);

    /* This is a bit tricky.
     * The Ref Count maybe zero for a free'd block. */
    if ((*bufp)->ref.ct <= 0)
    {
        LOG_Printf(ERROR, "Reference count for *0x%lx is less than 0", (uint64_t) *bufp);
        exit(100);
    }
    (*bufp)->ref.ct --;

    if ((*bufp)->ref.ct == 0)
    {
        LOG_Printf(DEBUG, "Freeing word_list_t *0x%lx", (uint64_t) *bufp);
        memset((void *) (*bufp), 0, sizeof(word_list_t));
        free((void *) (*bufp));
    }

    return;
}


static void usage(char *pname)
{
    printf("%s <start word> <end word> <dictionary file>", pname);
    return;
}


int main(int argc, char **argv)
{
    int ret = 0;
    char *start = NULL;
    char *end = NULL;
    char *dict_name = NULL;
    int len = 0;

    ENV_EnQueuePrint = getenv("WORD_LADDER_PRINT_QUEUE");

    /* This should be an allocator, maybe a macro? */
    word_list_t *wl = malloc(sizeof(word_list_t));
    memset((void *) wl, 0, sizeof(word_list_t));
    wl->ref.marker = 0xa5a5a5a5;
    SharedPtr(word_list_t, wl);

    /* Simple args check. 
     * TODO Better checks. */
    if (argc != 4)
    {
        usage(argv[0]);
        return(EINVAL);
    }

    start = argv[1];
    end = argv[2];
    dict_name = argv[3];

    LOG_Printf(WARNING, "TODO: Free each tree for entire tree");

    LOG_Printf(INFO, "Using dictionary file: %s", dict_name);
    wl->fname = dict_name;
    ret = wl_read_list(wl);
    RetOnErrorWithLog(ret, "Dictionary file access error.");

    LOG_Printf(INFO, "Starting from \"%s\" to \"%s\" through \"%s\"...", start, end, argv[3]);
    ret = word_traverse(start, end, wl, &len);
    RetOnErrorWithLog(ret, "Look up failed");

    return SUCCESS;
}


int word_traverse(char *start, char *end, word_list_t *wl, int *len)
{
    int ret = 0;
    int pos = 0;
    int ii = 0;
    int iter = 0;
    char ch = 'a';
    char *t_w = NULL;
    char *curr_w = NULL;
    q_t *word_q = NULL;
    q_node_t *tmp_n = NULL;
    q_node_t *curr_n = NULL;
    CleanUp uint8_t *valid_bm = NULL;
    SharedPtr(word_list_t, wl);

    word_q = malloc(sizeof(q_t));
    memset(word_q, 0, sizeof(q_t));

    valid_bm = malloc((wl->n_words / 8) + 1);
    memset(valid_bm, 0xff, (wl->n_words / 8) + 1);

    ret = sanity_checks(start, end, wl, valid_bm);
    ReturnOnError(ret);

    /* Start with queue */
    ret = q_init(word_q);
    ReturnOnError(ret);

    curr_w = strndup(start, strlen(start));
    ret = q_init_node(&curr_n, (void*) curr_w);
    ReturnOnError(ret);

    t_w = strndup(start, strlen(start));
    while (strncasecmp(curr_w, end, strlen(curr_w)) != 0)
    {
        /* Algorithm:
         * ----------
         * For all possible letter combinations, find all possible
         * valid words from the dictionary. For all possible words
         * from the list, perform an exhaustive BFS until the
         * required word is found. */
        for (ii = 0; ii < strlen(start); ii++)
        {
            strncpy(t_w, curr_w, strlen(start));
            for (ch = 'a'; ch <= 'z'; ch++)
            {
                t_w[ii] = ch;
                
                if (wl_lookup(wl, t_w, &pos, valid_bm) != SUCCESS)
                {
                    continue;
                }

                if (strncasecmp(t_w, end, strlen(t_w)) == 0)
                {
                    tmp_n = curr_n;
                    iter += 1;
                    printf("%s", end);
                    while(tmp_n->parent != NULL)
                    {
                        printf(" <- %s", (char *) tmp_n->data);
                        tmp_n = tmp_n->parent;
                        iter += 1;
                    }
                    printf(" <- %s\n", start);

                    LOG_Printf(INFO, "Total hops: %d", iter);
                    return SUCCESS;
                }

                ret = q_init_node(&tmp_n, (void*) strndup(t_w, strlen(start)));
                ReturnOnError(ret);

                tmp_n->parent = curr_n;
                ret = q_fifo_in(word_q, tmp_n);
                ReturnOnError(ret);

                uint8_t bitmask = (1 << (pos & 0x07));
                valid_bm[pos >> 3] &= (~bitmask);
            }
        }

        debug_print_q(word_q);

        ret = q_fifo_out(word_q, &curr_n);
        ReturnOnError(ret);

        curr_w = (char*) curr_n->data;
        LOG_Printf(INFO, "Checking word combo for: %s ->...-> %s", curr_w, end);
    }

    return(ret);
}


int sanity_checks(char *start, char *end, word_list_t *wl, uint8_t *valid_bm)
{
    int ii = 0;
    int pos = 0;
    int ret = SUCCESS;
    SharedPtr(word_list_t, wl);

    /* Preliminary checks */
    if ((start == NULL) ||
            (end == NULL))
    {
        E_Printf("Invalid inputs to word_traverse\n");
        return(EINVAL);
    }

    if (strlen(start) != strlen(end))
    {
        LOG_Printf(ERROR, "This implemenation only supports words of same size.");
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

    if (wl_lookup(wl, end, &pos, valid_bm) != 0)
    {
        LOG_Printf(ERROR, "Given word(s) are not in the dictionary.");
        return(ENOTSUP);
    }

    return(ret);
}


static void debug_print_q(q_t *q)
{
    int ii = 0;
    q_node_t *node = NULL;

    if (ENV_EnQueuePrint == NULL)
    {
        return;
    }

    printf("Queue of %d nodes at 0x%08lx: ", q->len, ((long) q));
    
    if (q == NULL)
    {
        LOG_Printf(WARNING, "Invalid queue");
        return;
    }
    
    node = q->head;
    for (ii = 0; ii < q->len; ii++)
    {
        printf(" -> %s", (char*) node->data);
        if (node->next == NULL)
        {
            break;
        }
        node = node->next;
    }
    
    printf("\n");
    return;
}


void cleanup_and_exit(int retcode)
{
    exit(retcode);
}


/* End of file */
