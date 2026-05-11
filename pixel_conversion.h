/**************************************************************
 *                     pixel_conversion.h
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This header file declares functions for converting between RGB and YPbPr
 *   color spaces, as well as quantization and unquantization of DCT 
 *   coefficients.
 *
**************************************************************/

#ifndef PIXEL_CONVERSIONS_H
#define PIXEL_CONVERSIONS_H

#include <pnm.h>
#include <stdint.h>

/* Structures for color space and DCT coefficients */

/*
 * name:       ypbpr
 * purpose:    A struct to hold the Y, Pb, and Pr components of a pixel 
 *             in the YPbPr color space. 
 * parameters: y:  the luminance component
 *             pb: the blue-difference chroma component
 *             pr: the red-difference chroma component
 * return:     None.
*/
struct ypbpr { float y, pb, pr; };

/*
 * name:       dct_coeffs
 * purpose:    A struct to hold the four coefficients (a, b, c, d) 
 *             resulting from 
 * parameters: a: the average luminance of the block
 *             b: the horizontal frequency component
 *             c: the vertical frequency component
 *             d: the diagonal frequency component
 * return:     None.
*/
struct dct_coeffs { float a, b, c, d; };

/*
 * name:       rgb_to_ypbpr
 * purpose:    Converts a single RGB pixel to the YPbPr color space.
 * parameters: rgb:   the struct containing red, green, and blue components
 *             denom: the denominator used to scale the integer RGB values
 * return:     A struct ypbpr containing floating point Y, Pb, and Pr values.
*/
struct ypbpr rgb_to_ypbpr(struct Pnm_rgb rgb, unsigned denom);

/*
 * name:       ypbpr_to_rgb
 * purpose:    Converts a YPbPr pixel back to the RGB color space.
 * parameters: pix:   the struct containing Y, Pb, and Pr floating point values
 *             denom: the target denominator for the resulting RGB pixel
 * return:     A struct Pnm_rgb scaled by the provided denominator.
*/
struct Pnm_rgb ypbpr_to_rgb(struct ypbpr pix, unsigned denom);

/*
 * name:       quantize_small
 * purpose:    Quantizes a floating point DCT coefficient (b, c, or d).
 * parameters: f: the floating point coefficient to quantize
 * return:     An integer representation scaled to a 5-bit range.
*/
int quantize_small(float f);

/*
 * name:       unquantize_small
 * purpose:    Converts a quantized 5-bit integer back to a float.
 * parameters: n: the quantized integer value
 * return:     The floating point representation of the coefficient.
*/
float unquantize_small(int n);

#endif
