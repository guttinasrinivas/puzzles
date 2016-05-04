#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
#include "word_list.h"
#include "word_grid.h"

static void usage(char *pname)
{
    printf("%s <word list> <grid as 16 characters>\n", pname);
    return;
}

int main(int argc, char **argv)
{
    int ret = 0;
    char *grid = NULL;
    char *dict_name = NULL;
    word_list_t wl;
    
    /* Simple args check. */
	if (argc != 3)
    {
        usage(argv[0]);
        return(E_ARGS);
    }
    else
    {
        dict_name = argv[1];
        grid = argv[2];
    }
    
    wl.fname = dict_name;
    wl.filters = grid;
    ret = wl_read_list(&wl);
    RET_MSG_ON_ERR(ret, "Dictionary file access error.\n");
    
    LOG_PRINTF(INFO, "Starting up...\n");
    ret = wg_word_build(grid, &wl);
    RET_MSG_ON_ERR(ret, "Error %d finding words in given input.\n", ret);

    LOG_PRINTF(INFO, "Done.\n");
    
	return(ret);
}

