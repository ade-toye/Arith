/**************************************************************
 *                     40image.c
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This program serves as the main entry point for the image 
 *   compression and decompression. It parses command-line 
 *   arguments to determine whether to compress or decompress an image, 
 *   and then delegates the actual processing to the appropriate function.
 *.  The program supports reading from a specified file or from
 *   standard input, and writes the output to standard output.
 * 
 *
**************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"

static void (*compress_or_decompress)(FILE *input) = compress40;

/*
 * name:      usage_and_die
 * purpose:   Print the required command-line usage message for 40image
 *            to stderr and terminate the program with a failure status.
 * parameters: none
 * return:    Does not return (exits the program).
 * expect:    Always called only when the command-line arguments are invalid.
 * notes:     none.
 */
static void usage_and_die(void)
{
    fprintf(stderr,
            "Usage: 40image -d [filename]\n"
            "40image -c [filename]\n");
    exit(1);
}


/*
 * name:      main
 * purpose:   Parse command-line arguments to select either compression (-c)
 *            or decompression (-d), open an optional input file (or use stdin),
 *            and invoke the selected operation. Output is written to stdout
 *            by the selected operation.
 * parameters: argc - number of command-line arguments
 *            argv - array of command-line argument strings
 * return:    EXIT_SUCCESS (0) on successful completion.
 * expect:    At most one non-option argument (a filename). Any invalid option
 *            or invalid argument pattern triggers usage_and_die().
 *            If a filename is provided, it must be openable for reading.
 * notes:     none.
 */
int main(int argc, char *argv[])
{
        int i;
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        usage_and_die();
                } else if (argc - i > 2) {
                        usage_and_die();
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }
        return EXIT_SUCCESS; 
}
