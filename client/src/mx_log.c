#include "../inc/uchat.h"

static FILE *log_fp = NULL;

void mx_init_logging(char *filename) {
    log_fp = fopen(filename, "a+");
    log_add_fp(log_fp, 0);
}

void mx_end_logging(void) {
    if (log_fp != NULL) {
        fclose(log_fp);
        log_fp = NULL;
    }
}
