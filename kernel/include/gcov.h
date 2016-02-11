#ifndef __POK_KERNEL_GCOV_H__
#define __POK_KERNEL_GCOV_H__

#include <config.h>

#ifdef POK_NEEDS_GCOV

#if __GNUC__ == 5 && __GNUC_MINOR__ >= 1
#define GCOV_COUNTERS           10
#elif __GNUC__ == 4 && __GNUC_MINOR__ >= 9
#define GCOV_COUNTERS           9
#else
#define GCOV_COUNTERS           8
#endif

#define GCOV_TAG_FUNCTION_LENGTH    3

/*
 * Profiling data types used for gcc 3.4 and above - these are defined by
 * gcc and need to be kept as close to the original definition as possible to
 * remain compatible.
 */
#define GCOV_DATA_MAGIC     ((unsigned int) 0x67636461)
#define GCOV_TAG_FUNCTION   ((unsigned int) 0x01000000)
#define GCOV_TAG_COUNTER_BASE   ((unsigned int) 0x01a10000)
#define GCOV_TAG_FOR_COUNTER(count)                 \
    (GCOV_TAG_COUNTER_BASE + ((unsigned int) (count) << 17))

#if BITS_PER_LONG >= 64
typedef long gcov_type;
#else
typedef long long gcov_type;
#endif


/**
 * struct gcov_ctr_info - information about counters for a single function
 * @num: number of counter values for this type
 * @values: array of counter values for this type
 *
 * This data is generated by gcc during compilation and doesn't change
 * at run-time with the exception of the values array.
 */
struct gcov_ctr_info {
    unsigned int num;
    gcov_type *values;
};

/**
 * struct gcov_fn_info - profiling meta data per function
 * @key: comdat key
 * @ident: unique ident of function
 * @lineno_checksum: function lineo_checksum
 * @cfg_checksum: function cfg checksum
 * @ctrs: instrumented counters
 *
 * This data is generated by gcc during compilation and doesn't change
 * at run-time.
 *
 * Information about a single function.  This uses the trailing array
 * idiom. The number of counters is determined from the merge pointer
 * array in gcov_info.  The key is used to detect which of a set of
 * comdat functions was selected -- it points to the gcov_info object
 * of the object file containing the selected comdat function.
 */
struct gcov_fn_info {
    const struct gcov_info *key;
    unsigned int ident;
    unsigned int lineno_checksum;
    unsigned int cfg_checksum;
    struct gcov_ctr_info ctrs[0];
};

/**
 * struct gcov_info - profiling data per object file
 * @version: gcov version magic indicating the gcc version used for compilation
 * @next: list head for a singly-linked list
 * @stamp: uniquifying time stamp
 * @filename: name of the associated gcov data file
 * @merge: merge functions (null for unused counter type)
 * @n_functions: number of instrumented functions
 * @functions: pointer to pointers to function information
 *
 * This data is generated by gcc during compilation and doesn't change
 * at run-time with the exception of the next pointer.
 */
struct gcov_info {
    unsigned int version;
    struct gcov_info *next;
    unsigned int stamp;
    const char *filename;
    void (*merge[GCOV_COUNTERS])(gcov_type *, unsigned int);
    unsigned int n_functions;
    struct gcov_fn_info **functions;
};

// call the coverage initializers if not done by startup code
void pok_gcov_init(void);

// called by coverage initializers
void __gcov_init(struct gcov_info *info);

void gcov_dump(void);
#endif /* POK_NEEDS_GCOV */

#endif /* __POK_KERNEL_GCOV_H__ */
