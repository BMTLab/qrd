#ifndef _DIRENT_H
#define _DIRENT_H

#ifdef defined(_WIN32) || defined(WIN32)

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_IX86)
#define _X86_
#endif
#include <stdio.h>
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/* Indicates that d_type field is available in dirent structure */
#define _DIRENT_HAVE_D_TYPE

/* Indicates that d_namlen field is available in dirent structure */
#define _DIRENT_HAVE_D_NAMLEN

/* Entries missing from MSVC 6.0 */
#if !defined(FILE_ATTRIBUTE_DEVICE)
#define FILE_ATTRIBUTE_DEVICE 0x40
#endif

/* File type and permission flags for stat() */
#if !defined(S_IFMT)
#define S_IFMT _S_IFMT /* File type mask */
#endif
#if !defined(S_IFDIR)
#define S_IFDIR _S_IFDIR /* Directory */
#endif
#if !defined(S_IFCHR)
#define S_IFCHR _S_IFCHR /* Character device */
#endif
#if !defined(S_IFFIFO)
#define S_IFFIFO _S_IFFIFO /* Pipe */
#endif
#if !defined(S_IFREG)
#define S_IFREG _S_IFREG /* Regular file */
#endif
#if !defined(S_IREAD)
#define S_IREAD _S_IREAD /* Read permission */
#endif
#if !defined(S_IWRITE)
#define S_IWRITE _S_IWRITE /* Write permission */
#endif
#if !defined(S_IEXEC)
#define S_IEXEC _S_IEXEC /* Execute permission */
#endif
#if !defined(S_IFIFO)
#define S_IFIFO _S_IFIFO /* Pipe */
#endif
#if !defined(S_IFBLK)
#define S_IFBLK 0 /* Block device */
#endif
#if !defined(S_IFLNK)
#define S_IFLNK 0 /* Link */
#endif
#if !defined(S_IFSOCK)
#define S_IFSOCK 0 /* Socket */
#endif

#if defined(_MSC_VER)
#define S_IRUSR S_IREAD  /* Read user */
#define S_IWUSR S_IWRITE /* Write user */
#define S_IXUSR 0        /* Execute user */
#define S_IRGRP 0        /* Read group */
#define S_IWGRP 0        /* Write group */
#define S_IXGRP 0        /* Execute group */
#define S_IROTH 0        /* Read others */
#define S_IWOTH 0        /* Write others */
#define S_IXOTH 0        /* Execute others */
#endif

/* Maximum length of file name */
#if !defined(PATH_MAX)
#define PATH_MAX MAX_PATH
#endif
#if !defined(FILENAME_MAX)
#define FILENAME_MAX MAX_PATH
#endif
#if !defined(NAME_MAX)
#define NAME_MAX FILENAME_MAX
#endif

/* File type flags for d_type */
#define DT_UNKNOWN 0
#define DT_REG S_IFREG
#define DT_DIR S_IFDIR
#define DT_FIFO S_IFIFO
#define DT_SOCK S_IFSOCK
#define DT_CHR S_IFCHR
#define DT_BLK S_IFBLK

/* Macros for converting between st_mode and d_type */
#define IFTODT(mode) ((mode)&S_IFMT)
#define DTTOIF(type) (type)

/*
 * File type macros.  Note that block devices, sockets and links cannot be
 * distinguished on Windows and the macros S_ISBLK, S_ISSOCK and S_ISLNK are
 * only defined for compatibility.  These macros should always return false
 * on Windows.
 */
#define S_ISFIFO(mode) (((mode)&S_IFMT) == S_IFIFO)
#define S_ISDIR(mode) (((mode)&S_IFMT) == S_IFDIR)
#define S_ISREG(mode) (((mode)&S_IFMT) == S_IFREG)
#define S_ISLNK(mode) (((mode)&S_IFMT) == S_IFLNK)
#define S_ISSOCK(mode) (((mode)&S_IFMT) == S_IFSOCK)
#define S_ISCHR(mode) (((mode)&S_IFMT) == S_IFCHR)
#define S_ISBLK(mode) (((mode)&S_IFMT) == S_IFBLK)

/* Return the exact length of d_namlen without zero terminator */
#define _D_EXACT_NAMLEN(p) ((p)->d_namlen)

/* Return number of bytes needed to store d_namlen */
#define _D_ALLOC_NAMLEN(p) (PATH_MAX + 1)

#ifdef __cplusplus
extern "C"
{
#endif

    /* Wide-character version */
    struct _wdirent
    {
        long d_ino;                   /* Always zero */
        unsigned short d_reclen;      /* Structure size */
        size_t d_namlen;              /* Length of name without \0 */
        int d_type;                   /* File type */
        wchar_t d_name[PATH_MAX + 1]; /* File name */
    };
    typedef struct _wdirent _wdirent;

    struct _WDIR
    {
        struct _wdirent ent;   /* Current directory entry */
        WIN32_FIND_DATAW data; /* Private file data */
        int cached;            /* True if data is valid */
        HANDLE handle;         /* Win32 search handle */
        wchar_t *patt;         /* Initial directory name */
    };
    typedef struct _WDIR _WDIR;

    static _WDIR *_wopendir(const wchar_t *dirname);
    static struct _wdirent *_wreaddir(_WDIR *dirp);
    static int _wclosedir(_WDIR *dirp);
    static void _wrewinddir(_WDIR *dirp);

/* For compatibility with Symbian */
#define wdirent _wdirent
#define WDIR _WDIR
#define wopendir _wopendir
#define wreaddir _wreaddir
#define wclosedir _wclosedir
#define wrewinddir _wrewinddir

    /* Multi-byte character versions */
    struct dirent
    {
        long d_ino;                /* Always zero */
        unsigned short d_reclen;   /* Structure size */
        size_t d_namlen;           /* Length of name without \0 */
        int d_type;                /* File type */
        char d_name[PATH_MAX + 1]; /* File name */
    };
    typedef struct dirent dirent;

    struct DIR
    {
        struct dirent ent;
        struct _WDIR *wdirp;
    };
    typedef struct DIR DIR;

    static DIR *opendir(const char *dirname);
    static struct dirent *readdir(DIR *dirp);
    static int closedir(DIR *dirp);
    static void rewinddir(DIR *dirp);

    /* Internal utility functions */
    static WIN32_FIND_DATAW *dirent_first(_WDIR *dirp);
    static WIN32_FIND_DATAW *dirent_next(_WDIR *dirp);

    static int dirent_mbstowcs_s(
        size_t *pReturnValue,
        wchar_t *wcstr,
        size_t sizeInWords,
        const char *mbstr,
        size_t count);

    static int dirent_wcstombs_s(
        size_t *pReturnValue,
        char *mbstr,
        size_t sizeInBytes,
        const wchar_t *wcstr,
        size_t count);

    static void dirent_set_errno(int error);

    /*
 * Open directory stream DIRNAME for read and return a pointer to the
 * internal working area that is used to retrieve individual directory
 * entries.
 */
    static _WDIR *
    _wopendir(
        const wchar_t *dirname)
    {
        _WDIR *dirp = NULL;
        int error;

        /* Must have directory name */
        if (dirname == NULL || dirname[0] == '\0')
        {
            dirent_set_errno(ENOENT);
            return NULL;
        }

        /* Allocate new _WDIR structure */
        dirp = (_WDIR *)malloc(sizeof(struct _WDIR));
        if (dirp != NULL)
        {
            DWORD n;

            /* Reset _WDIR structure */
            dirp->handle = INVALID_HANDLE_VALUE;
            dirp->patt = NULL;
            dirp->cached = 0;

            /* Compute the length of full path plus zero terminator */
            n = GetFullPathNameW(dirname, 0, NULL, NULL);

            /* Allocate room for absolute directory name and search pattern */
            dirp->patt = (wchar_t *)malloc(sizeof(wchar_t) * n + 16);
            if (dirp->patt)
            {

                /*
             * Convert relative directory name to an absolute one.  This
             * allows rewinddir() to function correctly even when current
             * working directory is changed between opendir() and rewinddir().
             */
                n = GetFullPathNameW(dirname, n, dirp->patt, NULL);
                if (n > 0)
                {
                    wchar_t *p;

                    /* Append search pattern \* to the directory name */
                    p = dirp->patt + n;
                    if (dirp->patt < p)
                    {
                        switch (p[-1])
                        {
                        case '\\':
                        case '/':
                        case ':':
                            /* Directory ends in path separator, e.g. c:\temp\ */
                            /*NOP*/;
                            break;

                        default:
                            /* Directory name doesn't end in path separator */
                            *p++ = '\\';
                        }
                    }
                    *p++ = '*';
                    *p = '\0';

                    /* Open directory stream and retrieve the first entry */
                    if (dirent_first(dirp))
                    {
                        /* Directory stream opened successfully */
                        error = 0;
                    }
                    else
                    {
                        /* Cannot retrieve first entry */
                        error = 1;
                        dirent_set_errno(ENOENT);
                    }
                }
                else
                {
                    /* Cannot retrieve full path name */
                    dirent_set_errno(ENOENT);
                    error = 1;
                }
            }
            else
            {
                /* Cannot allocate memory for search pattern */
                error = 1;
            }
        }
        else
        {
            /* Cannot allocate _WDIR structure */
            error = 1;
        }

        /* Clean up in case of error */
        if (error && dirp)
        {
            _wclosedir(dirp);
            dirp = NULL;
        }

        return dirp;
    }

    /*
 * Read next directory entry.  The directory entry is returned in dirent
 * structure in the d_name field.  Individual directory entries returned by
 * this function include regular files, sub-directories, pseudo-directories
 * "." and ".." as well as volume labels, hidden files and system files.
 */
    static struct _wdirent *
    _wreaddir(
        _WDIR *dirp)
    {
        WIN32_FIND_DATAW *datap;
        struct _wdirent *entp;

        /* Read next directory entry */
        datap = dirent_next(dirp);
        if (datap)
        {
            size_t n;
            DWORD attr;

            /* Pointer to directory entry to return */
            entp = &dirp->ent;

            /*
         * Copy file name as wide-character string.  If the file name is too
         * long to fit in to the destination buffer, then truncate file name
         * to PATH_MAX characters and zero-terminate the buffer.
         */
            n = 0;
            while (n < PATH_MAX && datap->cFileName[n] != 0)
            {
                entp->d_name[n] = datap->cFileName[n];
                n++;
            }
            dirp->ent.d_name[n] = 0;

            /* Length of file name excluding zero terminator */
            entp->d_namlen = n;

            /* File type */
            attr = datap->dwFileAttributes;
            if ((attr & FILE_ATTRIBUTE_DEVICE) != 0)
            {
                entp->d_type = DT_CHR;
            }
            else if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                entp->d_type = DT_DIR;
            }
            else
            {
                entp->d_type = DT_REG;
            }

            /* Reset dummy fields */
            entp->d_ino = 0;
            entp->d_reclen = sizeof(struct _wdirent);
        }
        else
        {

            /* Last directory entry read */
            entp = NULL;
        }

        return entp;
    }

    /*
 * Close directory stream opened by opendir() function.  This invalidates the
 * DIR structure as well as any directory entry read previously by
 * _wreaddir().
 */
    static int
    _wclosedir(
        _WDIR *dirp)
    {
        int ok;
        if (dirp)
        {

            /* Release search handle */
            if (dirp->handle != INVALID_HANDLE_VALUE)
            {
                FindClose(dirp->handle);
                dirp->handle = INVALID_HANDLE_VALUE;
            }

            /* Release search pattern */
            if (dirp->patt)
            {
                free(dirp->patt);
                dirp->patt = NULL;
            }

            /* Release directory structure */
            free(dirp);
            ok = /*success*/ 0;
        }
        else
        {
            /* Invalid directory stream */
            dirent_set_errno(EBADF);
            ok = /*failure*/ -1;
        }
        return ok;
    }

    /*
 * Rewind directory stream such that _wreaddir() returns the very first
 * file name again.
 */
    static void
    _wrewinddir(
        _WDIR *dirp)
    {
        if (dirp)
        {
            /* Release existing search handle */
            if (dirp->handle != INVALID_HANDLE_VALUE)
            {
                FindClose(dirp->handle);
            }

            /* Open new search handle */
            dirent_first(dirp);
        }
    }

    /* Get first directory entry (internal) */
    static WIN32_FIND_DATAW *
    dirent_first(
        _WDIR *dirp)
    {
        WIN32_FIND_DATAW *datap;

        /* Open directory and retrieve the first entry */
        dirp->handle = FindFirstFileW(dirp->patt, &dirp->data);
        if (dirp->handle != INVALID_HANDLE_VALUE)
        {

            /* a directory entry is now waiting in memory */
            datap = &dirp->data;
            dirp->cached = 1;
        }
        else
        {

            /* Failed to re-open directory: no directory entry in memory */
            dirp->cached = 0;
            datap = NULL;
        }
        return datap;
    }

    /* Get next directory entry (internal) */
    static WIN32_FIND_DATAW *
    dirent_next(
        _WDIR *dirp)
    {
        WIN32_FIND_DATAW *p;

        /* Get next directory entry */
        if (dirp->cached != 0)
        {

            /* A valid directory entry already in memory */
            p = &dirp->data;
            dirp->cached = 0;
        }
        else if (dirp->handle != INVALID_HANDLE_VALUE)
        {

            /* Get the next directory entry from stream */
            if (FindNextFileW(dirp->handle, &dirp->data) != FALSE)
            {
                /* Got a file */
                p = &dirp->data;
            }
            else
            {
                /* The very last entry has been processed or an error occured */
                FindClose(dirp->handle);
                dirp->handle = INVALID_HANDLE_VALUE;
                p = NULL;
            }
        }
        else
        {

            /* End of directory stream reached */
            p = NULL;
        }

        return p;
    }

    /*
 * Open directory stream using plain old C-string.
 */
    static DIR *
    opendir(
        const char *dirname)
    {
        struct DIR *dirp;
        int error;

        /* Must have directory name */
        if (dirname == NULL || dirname[0] == '\0')
        {
            dirent_set_errno(ENOENT);
            return NULL;
        }

        /* Allocate memory for DIR structure */
        dirp = (DIR *)malloc(sizeof(struct DIR));
        if (dirp)
        {
            wchar_t wname[PATH_MAX + 1];
            size_t n;

            /* Convert directory name to wide-character string */
            error = dirent_mbstowcs_s(
                &n, wname, PATH_MAX + 1, dirname, PATH_MAX);
            if (!error)
            {

                /* Open directory stream using wide-character name */
                dirp->wdirp = _wopendir(wname);
                if (dirp->wdirp)
                {
                    /* Directory stream opened */
                    error = 0;
                }
                else
                {
                    /* Failed to open directory stream */
                    error = 1;
                }
            }
            else
            {
                /*
             * Cannot convert file name to wide-character string.  This
             * occurs if the string contains invalid multi-byte sequences or
             * the output buffer is too small to contain the resulting
             * string.
             */
                error = 1;
            }
        }
        else
        {
            /* Cannot allocate DIR structure */
            error = 1;
        }

        /* Clean up in case of error */
        if (error && dirp)
        {
            free(dirp);
            dirp = NULL;
        }

        return dirp;
    }

    /*
 * Read next directory entry.
 *
 * When working with text consoles, please note that file names returned by
 * readdir() are represented in the default ANSI code page while any output to
 * console is typically formatted on another code page.  Thus, non-ASCII
 * characters in file names will not usually display correctly on console.  The
 * problem can be fixed in two ways: (1) change the character set of console
 * to 1252 using chcp utility and use Lucida Console font, or (2) use
 * _cprintf function when writing to console.  The _cprinf() will re-encode
 * ANSI strings to the console code page so many non-ASCII characters will
 * display correcly.
 */
    static struct dirent *readdir(DIR *dirp)
    {
        WIN32_FIND_DATAW *datap;
        struct dirent *entp;

        /* Read next directory entry */
        datap = dirent_next(dirp->wdirp);
        if (datap)
        {
            size_t n;
            int error;

            /* Attempt to convert file name to multi-byte string */
            error = dirent_wcstombs_s(
                &n, dirp->ent.d_name, MAX_PATH + 1, datap->cFileName, MAX_PATH);

            /*
         * If the file name cannot be represented by a multi-byte string,
         * then attempt to use old 8+3 file name.  This allows traditional
         * Unix-code to access some file names despite of unicode
         * characters, although file names may seem unfamiliar to the user.
         *
         * Be ware that the code below cannot come up with a short file
         * name unless the file system provides one.  At least
         * VirtualBox shared folders fail to do this.
         */
            if (error && datap->cAlternateFileName[0] != '\0')
            {
                error = dirent_wcstombs_s(
                    &n, dirp->ent.d_name, MAX_PATH + 1, datap->cAlternateFileName,
                    sizeof(datap->cAlternateFileName) /
                        sizeof(datap->cAlternateFileName[0]));
            }

            if (!error)
            {
                DWORD attr;

                /* Initialize directory entry for return */
                entp = &dirp->ent;

                /* Length of file name excluding zero terminator */
                entp->d_namlen = n - 1;

                /* File attributes */
                attr = datap->dwFileAttributes;
                if ((attr & FILE_ATTRIBUTE_DEVICE) != 0)
                {
                    entp->d_type = DT_CHR;
                }
                else if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
                {
                    entp->d_type = DT_DIR;
                }
                else
                {
                    entp->d_type = DT_REG;
                }

                /* Reset dummy fields */
                entp->d_ino = 0;
                entp->d_reclen = sizeof(struct dirent);
            }
            else
            {
                /*
             * Cannot convert file name to multi-byte string so construct
             * an errornous directory entry and return that.  Note that
             * we cannot return NULL as that would stop the processing
             * of directory entries completely.
             */
                entp = &dirp->ent;
                entp->d_name[0] = '?';
                entp->d_name[1] = '\0';
                entp->d_namlen = 1;
                entp->d_type = DT_UNKNOWN;
                entp->d_ino = 0;
                entp->d_reclen = 0;
            }
        }
        else
        {
            /* No more directory entries */
            entp = NULL;
        }

        return entp;
    }

    /*
 * Close directory stream.
 */
    static int
    closedir(
        DIR *dirp)
    {
        int ok;
        if (dirp)
        {

            /* Close wide-character directory stream */
            ok = _wclosedir(dirp->wdirp);
            dirp->wdirp = NULL;

            /* Release multi-byte character version */
            free(dirp);
        }
        else
        {

            /* Invalid directory stream */
            dirent_set_errno(EBADF);
            ok = /*failure*/ -1;
        }
        return ok;
    }

    /*
 * Rewind directory stream to beginning.
 */
    static void
    rewinddir(
        DIR *dirp)
    {
        /* Rewind wide-character string directory stream */
        _wrewinddir(dirp->wdirp);
    }

    /* Convert multi-byte string to wide character string */
    static int
    dirent_mbstowcs_s(
        size_t *pReturnValue,
        wchar_t *wcstr,
        size_t sizeInWords,
        const char *mbstr,
        size_t count)
    {
        int error;

#if defined(_MSC_VER) && _MSC_VER >= 1400

        /* Microsoft Visual Studio 2005 or later */
        error = mbstowcs_s(pReturnValue, wcstr, sizeInWords, mbstr, count);

#else

    /* Older Visual Studio or non-Microsoft compiler */
    size_t n;

    /* Convert to wide-character string */
    n = mbstowcs(wcstr, mbstr, count);
    if (n < sizeInWords)
    {

        /* Zero-terminate output buffer */
        if (wcstr)
        {
            wcstr[n] = 0;
        }

        /* Length of resuting multi-byte string WITH zero terminator */
        if (pReturnValue)
        {
            *pReturnValue = n + 1;
        }

        /* Success */
        error = 0;
    }
    else
    {

        /* Could not convert string */
        error = 1;
    }

#endif

        return error;
    }

    /* Convert wide-character string to multi-byte string */
    static int
    dirent_wcstombs_s(
        size_t *pReturnValue,
        char *mbstr,
        size_t sizeInBytes,
        const wchar_t *wcstr,
        size_t count)
    {
        int error;

#if defined(_MSC_VER) && _MSC_VER >= 1400

        /* Microsoft Visual Studio 2005 or later */
        error = wcstombs_s(pReturnValue, mbstr, sizeInBytes, wcstr, count);

#else

    /* Older Visual Studio or non-Microsoft compiler */
    size_t n;

    /* Convert to multi-byte string */
    n = wcstombs(mbstr, wcstr, count);
    if (n < sizeInBytes)
    {

        /* Zero-terminate output buffer */
        if (mbstr)
        {
            mbstr[n] = '\0';
        }

        /* Lenght of resulting multi-bytes string WITH zero-terminator */
        if (pReturnValue)
        {
            *pReturnValue = n + 1;
        }

        /* Success */
        error = 0;
    }
    else
    {

        /* Cannot convert string */
        error = 1;
    }

#endif

        return error;
    }

    /* Set errno variable */
    static void
    dirent_set_errno(
        int error)
    {
#if defined(_MSC_VER)

        /* Microsoft Visual Studio */
        _set_errno(error);

#else

    /* Non-Microsoft compiler */
    errno = error;

#endif
    }

#ifdef __cplusplus
}
#endif

#elif __unix__

#include <features.h>
__BEGIN_DECLS
#include <bits/types.h>
#ifdef __USE_XOPEN
#ifndef __ino_t_defined
#ifndef __USE_FILE_OFFSET64
typedef __ino_t ino_t;
#else
typedef __ino64_t ino_t;
#endif
#define __ino_t_defined
#endif
#if defined __USE_LARGEFILE64 && !defined __ino64_t_defined
typedef __ino64_t ino64_t;
#define __ino64_t_defined
#endif
#endif

#include <bits/dirent.h>
#if (defined __USE_BSD || defined __USE_MISC) && !defined d_fileno
#define d_ino d_fileno /* Backward compatibility.  */
#endif

#ifdef _DIRENT_HAVE_D_NAMLEN
#define _D_EXACT_NAMLEN(d) ((d)->d_namlen)
#define _D_ALLOC_NAMLEN(d) (_D_EXACT_NAMLEN(d) + 1)
#else
#define _D_EXACT_NAMLEN(d) (strlen((d)->d_name))
#ifdef _DIRENT_HAVE_D_RECLEN
#define _D_ALLOC_NAMLEN(d) (((char *)(d) + (d)->d_reclen) - &(d)->d_name[0])
#else
#define _D_ALLOC_NAMLEN(d) (sizeof(d)->d_name > 1 ? sizeof(d)->d_name : _D_EXACT_NAMLEN(d) + 1)
#endif
#endif
#ifdef __USE_BSD

/* File types for `d_type'.  */
enum
{
    DT_UNKNOWN = 0,
#define DT_UNKNOWN DT_UNKNOWN
    DT_FIFO = 1,
#define DT_FIFO DT_FIFO
    DT_CHR = 2,
#define DT_CHR DT_CHR
    DT_DIR = 4,
#define DT_DIR DT_DIR
    DT_BLK = 6,
#define DT_BLK DT_BLK
    DT_REG = 8,
#define DT_REG DT_REG
    DT_LNK = 10,
#define DT_LNK DT_LNK
    DT_SOCK = 12,
#define DT_SOCK DT_SOCK
    DT_WHT = 14
#define DT_WHT DT_WHT
};

/* Convert between stat structure types and directory types.  */
#define IFTODT(mode) (((mode)&0170000) >> 12)
#define DTTOIF(dirtype) ((dirtype) << 12)
#endif

typedef struct __dirstream DIR;

extern DIR *opendir(__const char *__name) __nonnull((1));

#ifdef __USE_GNU
extern DIR *fdopendir(int __fd);
#endif

extern int closedir(DIR *__dirp) __nonnull((1));

#ifndef __USE_FILE_OFFSET64
extern struct dirent *readdir(DIR *__dirp) __nonnull((1));
#else
#ifdef __REDIRECT
extern struct dirent *__REDIRECT(readdir, (DIR * __dirp), readdir64)
    __nonnull((1));
#else
#define readdir readdir64
#endif
#endif

#ifdef __USE_LARGEFILE64
extern struct dirent64 *readdir64(DIR *__dirp) __nonnull((1));
#endif

#if defined __USE_POSIX || defined __USE_MISC

#ifndef __USE_FILE_OFFSET64
extern int readdir_r(DIR *__restrict __dirp,
                     struct dirent *__restrict __entry,
                     struct dirent **__restrict __result)
    __nonnull((1, 2, 3));
#else
#ifdef __REDIRECT
extern int __REDIRECT(readdir_r,
                      (DIR * __restrict __dirp,
                       struct dirent *__restrict __entry,
                       struct dirent **__restrict __result),
                      readdir64_r) __nonnull((1, 2, 3));
#else
#define readdir_r readdir64_r
#endif
#endif
#ifdef __USE_LARGEFILE64
extern int readdir64_r(DIR *__restrict __dirp,
                       struct dirent64 *__restrict __entry,
                       struct dirent64 **__restrict __result)
    __nonnull((1, 2, 3));
#endif
#endif

extern void rewinddir(DIR *__dirp) __THROW __nonnull((1));

#if defined __USE_BSD || defined __USE_MISC || defined __USE_XOPEN
#include <bits/types.h>
/* Seek to position POS on DIRP.  */
extern void seekdir(DIR *__dirp, long int __pos) __THROW __nonnull((1));
/* Return the current position of DIRP.  */
extern long int telldir(DIR *__dirp) __THROW __nonnull((1));
#endif
#if defined __USE_BSD || defined __USE_MISC
/* Return the file descriptor used by DIRP.  */
extern int dirfd(DIR *__dirp) __THROW __nonnull((1));
#if defined __OPTIMIZE__ && defined _DIR_dirfd
#define dirfd(dirp) _DIR_dirfd(dirp)
#endif
#ifndef MAXNAMLEN

#include <bits/posix1_lim.h>

#ifdef NAME_MAX
#define MAXNAMLEN NAME_MAX
#else
#define MAXNAMLEN 255
#endif
#endif
#define __need_size_t
#include <stddef.h>

#ifndef __USE_FILE_OFFSET64
extern int scandir(__const char *__restrict __dir,
                   struct dirent ***__restrict __namelist,
                   int (*__selector)(__const struct dirent *),
                   int (*__cmp)(__const void *, __const void *))
    __nonnull((1, 2));
#else
#ifdef __REDIRECT
extern int __REDIRECT(scandir,
                      (__const char *__restrict __dir,
                       struct dirent ***__restrict __namelist,
                       int (*__selector)(__const struct dirent *),
                       int (*__cmp)(__const void *, __const void *)),
                      scandir64) __nonnull((1, 2));
#else
#define scandir scandir64
#endif
#endif
#if defined __USE_GNU && defined __USE_LARGEFILE64

extern int scandir64(__const char *__restrict __dir,
                     struct dirent64 ***__restrict __namelist,
                     int (*__selector)(__const struct dirent64 *),
                     int (*__cmp)(__const void *, __const void *))
    __nonnull((1, 2));
#endif

#ifndef __USE_FILE_OFFSET64
extern int alphasort(__const void *__e1, __const void *__e2)
    __THROW __attribute_pure__ __nonnull((1, 2));
#else
#ifdef __REDIRECT
extern int __REDIRECT_NTH(alphasort,
                          (__const void *__e1, __const void *__e2),
                          alphasort64) __attribute_pure__ __nonnull((1, 2));
#else
#define alphasort alphasort64
#endif
#endif
#if defined __USE_GNU && defined __USE_LARGEFILE64
extern int alphasort64(__const void *__e1, __const void *__e2)
    __THROW __attribute_pure__ __nonnull((1, 2));
#endif
#ifdef __USE_GNU

#ifndef __USE_FILE_OFFSET64
extern int versionsort(__const void *__e1, __const void *__e2)
    __THROW __attribute_pure__ __nonnull((1, 2));
#else
#ifdef __REDIRECT
extern int __REDIRECT_NTH(versionsort,
                          (__const void *__e1, __const void *__e2),
                          versionsort64)
    __attribute_pure__ __nonnull((1, 2));
#else
#define versionsort versionsort64
#endif
#endif
#ifdef __USE_LARGEFILE64
extern int versionsort64(__const void *__e1, __const void *__e2)
    __THROW __attribute_pure__ __nonnull((1, 2));
#endif
#endif

#ifndef __USE_FILE_OFFSET64
extern __ssize_t getdirentries(int __fd, char *__restrict __buf,
                               size_t __nbytes,
                               __off_t *__restrict __basep)
    __THROW __nonnull((2, 4));
#else
#ifdef __REDIRECT
extern __ssize_t __REDIRECT_NTH(getdirentries,
                                (int __fd, char *__restrict __buf,
                                 size_t __nbytes,
                                 __off64_t *__restrict __basep),
                                getdirentries64) __nonnull((2, 4));
#else
#define getdirentries getdirentries64
#endif
#endif
#ifdef __USE_LARGEFILE64
extern __ssize_t getdirentries64(int __fd, char *__restrict __buf,
                                 size_t __nbytes,
                                 __off64_t *__restrict __basep)
    __THROW __nonnull((2, 4));
#endif
#endif /* Use BSD or misc.  */
__END_DECLS

#endif /*__unix__ */
#endif /* _DIRENT_H */
