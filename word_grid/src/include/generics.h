#ifndef __GENERICS_H__
#define __GENERICS_H__

/* Constants */

/* Error codes */
enum
{
    SUCCESS =               (0),
    E_ARGS =                (1),
    E_NOTSUP =              (2),
    E_NOTFOUND =            (3)
};

/* LOGGING STUFF */
/* LOG LEVELS */
#define CRITICAL    (1)
#define ERROR       (2)
#define WARNING     (3)
#define INFO        (4)
#define DEBUG       (5)

#define CURR_LOG_LEVEL  WARNING

#define LOG_PRINTF(lvl, ...)    if (lvl <= CURR_LOG_LEVEL) printf(__VA_ARGS__)
#define EPRINTF(...)            LOG_PRINTF(ERROR, __VA_ARGS__)
#define DBG_PRINTF(...)         LOG_PRINTF(DEBUG, __VA_ARGS__)


/* Macros */
#define RET_ON_ERR(ret)             if (ret != SUCCESS) return(ret);
#define RET_MSG_ON_ERR(ret, ...) \
if (ret != SUCCESS) \
{ \
    EPRINTF(__VA_ARGS__); \
    return(ret); \
}

#endif /* __GENERICS_H__ */
