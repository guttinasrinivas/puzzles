#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
#include "word_list.h"

/*
 * TODO:
 *  Clean up memory mapping and close fd.
 */

int wl_read_list(word_list_t *wl)
{
    int ret = 0;
    int ii = 0;
    char *buf = NULL;
    int tw = 0;
    int ii_alph = 0;
    int use_word = 0;
    
    ret = wl_map_file(wl);
    RET_ON_ERR(ret);
    buf = wl->map;
    
    /* Split the buffer into words */
    tw = 0;
    for (ii = 0; ii < wl->flen; ii++)
    {
        if (buf[ii] == '\n')
        {
            buf[ii] = '\0';
            tw++;
        }
    }
    
    wl->words = (char **)malloc(sizeof(char *) * tw);
    
    /* Split the words into array of strings */
    tw = 1;
    wl->words[0] = buf;
    
    for (ii = 0; ii < (wl->flen - 1); ii++)
    {
        for (ii_alph = 0; ii_alph < strlen(wl->filters); ii_alph++)
        {
            if (buf[ii] == wl->filters[ii_alph])
            {
                use_word = 1;
            }
        }

        if (buf[ii] == '\0')
        {
            wl->words[tw] = (buf + ii + 1);
            if (use_word == 1)
            {
                tw++;
                use_word = 0;
            }
        }
    }
    wl->n_words = tw;
    LOG_PRINTF(INFO, "Found %d words in dictionary %s.\n", tw, wl->fname);
    
    return(ret);
}


int wl_map_file(word_list_t *wl)
{
    int ret = 0;
    struct stat sb;
    
    wl->fd = open(wl->fname, O_RDONLY);
    if (wl->fd <= 0)
    {
        EPRINTF("Could not open dictionary %s: error %d.\n", wl->fname, errno);
        return(errno);
    }
    
    ret = fstat(wl->fd, &sb);
    if (ret != 0)
    {
        EPRINTF("%s: Access error.\n", wl->fname);
        return(errno);
    }
    
    wl->flen = sb.st_size;
    
    wl->map = (void *)mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE,
                         MAP_FILE|MAP_PRIVATE, wl->fd, 0);
    if (wl->map == MAP_FAILED)
    {
        EPRINTF("Could not access dictionary file.\n");
        return(ENOMEM);
    }
    
    return(ret);
}

int wl_cleanup(word_list_t *wl)
{
    int ret = SUCCESS;
    
    ret = munmap(wl->map, wl->flen);
    wl->flen = 0;
    wl->map = NULL;
    free(wl->words);
    wl->words = NULL;
    close(wl->fd);
    
    return(ret);
}

/* TODO Find a better look up algorithm.
 * Maybe, hash table???
 */
int wl_lookup(const word_list_t *wl, const char *word, int *pos)
{
    int ret = SUCCESS;
    int ii =0;
    
    for(ii = 0; ii < wl->n_words; ii++)
    {
        if (strcasecmp(word, wl->words[ii]) == 0)
        {
            //printf("Debug: %s == %s\n", word, wl->words[ii]);
            *pos = ii;
            return(ret);
        }
    }
    
    *pos = -1;
    ret = ENOENT;
    return(ret);
}

int wl_remove(const word_list_t *wl, int pos)
{
    int ret = SUCCESS;
    char *word = NULL;
    
    if ((pos < 0) ||
        (pos >= wl->n_words))
    {
        return(EINVAL);
    }
    
    word = wl->words[pos];
    word[0] = '\0';
    
    return(ret);
}
