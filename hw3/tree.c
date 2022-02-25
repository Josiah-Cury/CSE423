#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdarg.h>
#include "tree.h"
#include "j0gram.tab.h"

struct tree *alloc_tree(int prodrule, char *symbolname, int nkids, ...) {
    va_list arg;
    va_start(arg, nkids);

    /*while(){
        va_arg(arg, struct tree *);
    }*/

    return NULL;
}
