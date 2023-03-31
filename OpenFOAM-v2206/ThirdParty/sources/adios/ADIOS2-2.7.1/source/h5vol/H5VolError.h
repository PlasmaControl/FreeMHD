#ifndef ADIOS_ERR_H
#define ADIOS_ERR_H

#include <stdio.h>
#include <stdlib.h>

static void *safe_calloc(size_t n, size_t s, unsigned long line)
{
    if (n == 0)
    {
        return NULL;
    }

    void *p = calloc(n, s);
    if (!p)
    {
        fprintf(stderr, "[%s:%ld]Out of memory at calloc (%ld, %ld)\n",
                __FILE__, line, (unsigned long)n, (unsigned long)s);
        exit(EXIT_FAILURE);
    }
    return p;
}

#define SAFE_CALLOC(n, s) safe_calloc(n, s, __LINE__)

static void *safe_malloc(size_t n, unsigned long line)
{
    if (n == 0)
    {
        return NULL;
    }

    void *p = malloc(n);
    if (!p)
    {
        fprintf(stderr, "[%s:%ld]Out of memory at malloc (%ld bytes)\n",
                __FILE__, line, (unsigned long)n);
        exit(EXIT_FAILURE);
    }
    return p;
}

#define SAFE_MALLOC(n) safe_malloc(n, __LINE__)

static void safe_free(void *p)
{
    if (p == NULL)
    {
        return;
    }
    free(p);
    p = NULL;
}

#define SAFE_FREE(ptr) safe_free(ptr)

static void *safe_ralloc(void *ptr, size_t newsize, unsigned long line)
{
    void *p = realloc(ptr, newsize);
    if (!p)
    {
        fprintf(stderr, "[%s:%ld]Out of memory at ralloc to (%ld bytes)\n",
                __FILE__, line, (unsigned long)newsize);
        exit(EXIT_FAILURE);
    }
    return p;
}

#define SAFE_REALLOC(ptr, newsize) safe_ralloc(ptr, newsize, __LINE__)

#define ADIOS_VOL_LOG_ERR(...)                                                 \
    {                                                                          \
        fprintf(stderr, "## ADIOS_VOL_ERROR:");                                \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, " In function:: %s\n", __FUNCTION__);                  \
        fflush(stderr);                                                        \
    }

#define ADIOS_VOL_NOT_SUPPORTED_ERR(...)                                       \
    {                                                                          \
        fprintf(stderr, "## ADIOS_VOL_NOT_SUPPORTED:");                        \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, " In function:: %s\n", __FUNCTION__);                  \
        fflush(stderr);                                                        \
    }

#define ADIOS_VOL_NOT_SUPPORTED(...)                                           \
    {                                                                          \
        ADIOS_VOL_NOT_SUPPORTED_ERR(__VA_ARGS__);                              \
        return -1; /* return err */                                            \
    }

#define ADIOS_VOL_WARN(...)                                                    \
    {                                                                          \
        fprintf(stderr, " ## ADIOS VOL WARNING :");                            \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, " In function:: %s\n", __FUNCTION__);                  \
        fflush(stderr);                                                        \
    }
#define SHOW_ERROR_MSG(...)                                                    \
    {                                                                          \
        ADIOS_VOL_LOG_ERR(__VA_ARGS__);                                        \
    }

#define REQUIRE_NOT_NULL(x)                                                    \
    if (NULL == x)                                                             \
    {                                                                          \
        ADIOS_VOL_LOG_ERR("");                                                 \
        return;                                                                \
    }
#define REQUIRE_NOT_NULL_ERR(x, errReturn)                                     \
    if (NULL == x)                                                             \
    {                                                                          \
        ADIOS_VOL_LOG_ERR("");                                                 \
        return errReturn;                                                      \
    };

//#define REQUIRE_MPI_SUCC(err) if (err != MPI_SUCCESS)
//{ADIOS_VOL_MPI_ERR(err);}
#define REQUIRE_MPI_SUCC(err)                                                  \
    if (err != MPI_SUCCESS)                                                    \
    {                                                                          \
        ADIOS_VOL_MPI_ERR(err);                                                \
        return -1;                                                             \
    }

#define REQUIRE_SUCC(valid, errReturn)                                         \
    if (!valid)                                                                \
    {                                                                          \
        return errReturn;                                                      \
    }
#define REQUIRE_SUCC_MSG(valid, errReturn, ...)                                \
    if (!valid)                                                                \
    {                                                                          \
        SHOW_ERROR_MSG(__VA_ARGS__);                                           \
        return errReturn;                                                      \
    }
#define REQUIRE_SUCC_ACTION(valid, action, errReturn)                          \
    if (!valid)                                                                \
    {                                                                          \
        action;                                                                \
        return errReturn;                                                      \
    }

#endif
