#ifndef SCAN_FILES_H
#define SCAN_FILES_H

#ifdef USE_QRD_CONFIG
#ifndef QRD_DEMO_CONFIG_H
#include "qrd_demo_config.h"
#endif
#else
#define MAX_FILES 256
#define MAX_FILENAME_LENGTH 128
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    extern char file_names[MAX_FILES][MAX_FILENAME_LENGTH];
    extern inline const char* concat(const char *,  const char *);
    extern int scan_files(char *, int *);

#ifdef __cplusplus
}
#endif

#endif /* SCAN_FILES_H */
