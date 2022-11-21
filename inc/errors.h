#ifndef ERRORS_H
#define ERRORS_H

#define RET_NO_ERR 0
#define RET_ERR_MALLOC 1        /* Malloc failed */
#define RET_ERR_INIT 2          /* Init failed or never happened for module */
#define RET_ERR_IO 3            /* Issue with IO */
#define RET_ERR_PARAM 4         /* Parameter supplied to function not valid */
#define RET_ERR_THREAD 5        /* Error with threads (starting/exiting/sync) */
#define RET_ERR_UNDEFINED 255   /* Error not fitting any others */

#endif /* ifndef ERRORS_H*/
