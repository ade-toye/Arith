/**************************************************************
 *                     compress40.c
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This file implements the main compression and decompression functions for
 *   the image format. The compress40 function reads a PPM image, applies a
 *   discrete cosine transform and quantization to compress the image data, and
 *   writes the compressed binary data to stdout. The decompress40 function reads
 *   compressed binary data, reconstructs the image using inverse DCT, and writes
 *   the resulting PPM image to stdout.
 *
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "compress40.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "pixel_conversion.h"
#include "bitpack.h"
#include "arith40.h"
#include "assert.h"
#include "dct.h"

/*
 * name:       compress40
 * purpose:    Reads a PPM image from the input file, compresses it using
 *             discrete cosine transform and quantization, and prints the
 *             compressed binary data to stdout.
 * parameters: input: the open FILE pointer to read the PPM image from
 * return:     None.
 * expect:     input is a valid, open FILE pointer in a supported PPM format.
 * notes:      Trims the last row or column if the dimensions are odd.
*/
void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = Pnm_ppmread(input, methods);
        unsigned w = image->width - (image->width % 2);
        unsigned h = image->height - (image->height % 2);

        printf("COMP40 Compressed image format 2\n%u %u\n", w, h);

        /* Process the image in 2x2 blocks, applying DCT and quantization */
        for (unsigned r = 0; r < h; r += 2) {
                for (unsigned c = 0; c < w; c += 2) {
                        struct ypbpr p[4];
                        float apb = 0, apr = 0;

                        for (int i = 0; i < 4; i++) {
                                struct Pnm_rgb *rgb =
                                        methods->at(image->pixels,
                                                    c + (i % 2),
                                                    r + (i / 2));
                                p[i] = rgb_to_ypbpr(*rgb,
                                                   image->denominator);
                                apb += p[i].pb;
                                apr += p[i].pr;
                        }

                        struct dct_coeffs d =
                                apply_dct(p[0].y, p[1].y, p[2].y, p[3].y);

                        uint64_t word = 0;
                        float a = d.a;

                        if (a < 0.0) a = 0.0;
                        if (a > 1.0) a = 1.0;

                        /* Pack coefficients and chroma indices */
                        word = Bitpack_newu(word, 6, 26,
                                            (unsigned)round(a * 63.0));
                        word = Bitpack_news(word, 6, 20,
                                            quantize_small(d.b));
                        word = Bitpack_news(word, 6, 14,
                                            quantize_small(d.c));
                        word = Bitpack_news(word, 6, 8,
                                            quantize_small(d.d));
                        word = Bitpack_newu(word, 4, 4,
                                Arith40_index_of_chroma(apb / 4.0));
                        word = Bitpack_newu(word, 4, 0,
                                Arith40_index_of_chroma(apr / 4.0));

                        for (int i = 24; i >= 0; i -= 8) {
                                putchar((int)Bitpack_getu(word, 8, i));
                        }
                }
        }

        Pnm_ppmfree(&image);
}

/*
 * name:       decompress40
 * purpose:    Reads compressed binary data from the input file, performs
 *             decompression and inverse DCT, and writes the resulting PPM
 *             image to stdout.
 * parameters: input: the open FILE pointer to read compressed data from
 * return:     None.
 * expect:     input contains a valid header and binary data.
 * notes:      Allocates a blocked UArray2 to store the reconstructed pixels.
*/
void decompress40(FILE *input)
{
        unsigned w, h;
        int read = fscanf(input,
                          "COMP40 Compressed image format 2\n%u %u",
                          &w, &h);
        assert(read == 2);
        assert(getc(input) == '\n');
        A2Methods_T method = uarray2_methods_blocked;
        A2Methods_UArray2 px =
                method->new_with_blocksize(w, h, sizeof(struct Pnm_rgb), 2);
        /* Read each compressed block and reconstruct 2x2 pixels */
        for (unsigned r = 0; r < h; r += 2) {
                for (unsigned c = 0; c < w; c += 2) {
                        uint64_t word = 0;
                        for (int i = 24; i >= 0; i -= 8) {
                                int byte = getc(input);
                                assert(byte != EOF);
                                word = Bitpack_newu(word, 8, i,
                                                    (uint64_t)byte);
                        }
                        struct dct_coeffs d = {
                                (float)Bitpack_getu(word, 6, 26) / 63.0,
                                unquantize_small(Bitpack_gets(word, 6, 20)),
                                unquantize_small(Bitpack_gets(word, 6, 14)),
                                unquantize_small(Bitpack_gets(word, 6, 8))
                        };
                        float y[4];
                        float pb = Arith40_chroma_of_index(
                                        Bitpack_getu(word, 4, 4));
                        float pr = Arith40_chroma_of_index(
                                        Bitpack_getu(word, 4, 0));

                        inverse_dct(d, &y[0], &y[1], &y[2], &y[3]);

                        for (int i = 0; i < 4; i++) {
                                struct Pnm_rgb *rgb =
                                        method->at(px,
                                                   c + (i % 2),
                                                   r + (i / 2));
                                struct ypbpr cv = { y[i], pb, pr };
                                *rgb = ypbpr_to_rgb(cv, 255);
                        }
                }
        }
        /* Write reconstructed image to stdout and free the pixel array */
        struct Pnm_ppm res = {
                .width = w,
                .height = h,
                .denominator = 255,
                .pixels = px,
                .methods = method
        };
        Pnm_ppmwrite(stdout, &res);
        method->free(&px);
}