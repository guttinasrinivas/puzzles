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
#include "simple_logger.h"
#include "generics.h"
#include "word_list.h"
#include "word_grid.h"


int LOG_CurrLevel;


static int print_grid(const char *gridbuf)
{
    if (strlen(gridbuf) != 16) {
        LOG_Error("For now only 4x4 grid is supported. Cannot process given grid.");
        return E_ARGS;
    }

    for (int ii = 0; ii < 16; ii++) {
        if (ii%4 == 0) {
            printf("\n");
        }
        printf("%c", gridbuf[ii]);
    }

    printf("\n\n");

    return SUCCESS;
}


static void usage(char* pname)
{
    printf("%s <word list> <grid as 16 characters>\n", pname);
    return;
}

int main(int argc, char** argv)
{
    int ret = 0;
    char* grid = NULL;
    char* dict_name = NULL;
    word_list_t wl;

    /* Simple args check. */
    if (argc != 3) {
        usage(argv[0]);
        return (E_ARGS);
    } else {
        dict_name = argv[1];
        grid = argv[2];
    }

    ret = print_grid(argv[2]);
    ReturnOnError(ret);

    wl.fname = dict_name;
    wl.filters = grid;
    ret = wl_read_list(&wl);
    RetOnErrorWithLog(ret, "Dictionary file access error.");

    LOG_Info("Starting up...\n");
    ret = wg_word_build(grid, &wl);
    RetOnErrorWithLog(ret, "Error %d finding words in given input.", ret);

    LOG_Info("Done.\n");

    return (ret);
}


void cleanup_and_exit(int retcode)
{
    exit(retcode);
}


/* End of file */
