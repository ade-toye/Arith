/**************************************************************
 *                     ppmdiff.c
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This file implements the ppmdiff program, which compares two PPM 
 *   images and computes the root mean square (RMS) difference 
 *   between their pixels.
 *
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

/*
 * name:       main
 * purpose:    Compares two PPM images and outputs the root mean square 
 *             difference of their pixels to stdout.
 * parameters: argc: number of command line arguments
 *             argv: array of command line argument strings
 * return:     EXIT_SUCCESS on success, EXIT_FAILURE on error.
 * expect:     Two filenames or "-" for stdin. Width/height differ by <= 1.
 * notes:      Prints 1.0 to stdout and error to stderr if dimensions mismatch.
*/
int main(int argc, char *argv[]) 
{
        if (argc != 3) {
                fprintf(stderr, "Usage: %s <image1> <image2>\n", argv[0]);
                return EXIT_FAILURE;
        }

        /* Use plain array methods for image storage */
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        Pnm_ppm img1, img2;

        /* Open files and handle standard input case */
        FILE *fp1 = (strcmp(argv[1], "-") == 0) ? stdin : fopen(argv[1], "rb");
        FILE *fp2 = (strcmp(argv[2], "-") == 0) ? stdin : fopen(argv[2], "rb");

        if (fp1 == NULL || fp2 == NULL) {
                fprintf(stderr, "Error: Could not open one or both files.\n");
                return EXIT_FAILURE;
        }

        img1 = Pnm_ppmread(fp1, methods);
        img2 = Pnm_ppmread(fp2, methods);

        /* Validate dimensions: difference must be at most 1 */
        if (abs((int)img1->width - (int)img2->width) > 1 || 
            abs((int)img1->height - (int)img2->height) > 1) {
                fprintf(stderr, "Dimensions differ by more than 1\n");
                printf("1.0\n");
                Pnm_ppmfree(&img1);
                Pnm_ppmfree(&img2);
                return EXIT_FAILURE;
        }

        int w = (img1->width < img2->width) ? img1->width : img2->width;
        int h = (img1->height < img2->height) ? img1->height : img2->height;

        /* Compute Root Mean Square (RMS) difference */
        double sum = 0.0;
        for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                        Pnm_rgb p1 = img1->methods->at(img1->pixels, i, j);
                        Pnm_rgb p2 = img2->methods->at(img2->pixels, i, j);

                        double d1 = (double)img1->denominator;
                        double d2 = (double)img2->denominator;

                        double r = (double)p1->red/d1 - (double)p2->red/d2;
                        double g = (double)p1->green/d1 - (double)p2->green/d2;
                        double b = (double)p1->blue/d1 - (double)p2->blue/d2;

                        sum += (r * r) + (g * g) + (b * b);
                }
        }

        printf("%.4f\n", sqrt(sum / (3.0 * w * h)));

        /* Cleanup resources */
        Pnm_ppmfree(&img1);
        Pnm_ppmfree(&img2);
        if (fp1 != stdin) fclose(fp1);
        if (fp2 != stdin) fclose(fp2);

        return EXIT_SUCCESS;
}