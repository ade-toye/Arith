/**************************************************************
 *                     compress40.h
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This header file declares the main functions for compressing
 *   and decompressing.
 *
**************************************************************/
#ifndef COMPRESS40_H
#define COMPRESS40_H

#include <stdio.h>

/*
 * name:       compress40
 * purpose:    Reads a PPM image from the provided input file, compresses it
 *             using DCT and quantization, and writes the resulting binary
 *             data to stdout.
 * parameters: input: the open FILE pointer to read the PPM image from
 * return:     None.
 * expect:     input is a valid, open FILE pointer in a supported PPM format.
 * notes:      none.
*/
extern void compress40(FILE *input);

/*
 * name:       decompress40
 * purpose:    Reads compressed binary data from the provided input file, 
 *             reconstructs the image using inverse DCT, and writes the 
 *             resulting PPM image to stdout.
 * parameters: input: the open FILE pointer to read compressed data from
 * return:     None.
 * expect:     input contains a valid header and binary data.
 * notes:      The output is a standard PPM image with a denominator of 255.
*/
extern void decompress40(FILE *input);

#endif


