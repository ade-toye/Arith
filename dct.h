/**************************************************************
 *                     dct.h
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This header file declares the functions for applying the 
 *   discrete cosine transform
 *
**************************************************************/

#ifndef DCT_H
#define DCT_H

#include "pixel_conversion.h"

/* 
 * name:       apply_dct
 * purpose:    Transforms four luminance (Y) values from a 2x2 block into 
 *             cosine space coefficients (a, b, c, d).
 * parameters: y1: top-left luminance value
 *             y2: top-right luminance value
 *             y3: bottom-left luminance value
 *             y4: bottom-right luminance value
 * return:     A struct dct_coeffs containing the four calculated 
 *             coefficients.
 * expect:     None.
 * notes:      None.
*/
struct dct_coeffs apply_dct(float y1, float y2, float y3, float y4);

/* 
 * name:       inverse_dct
 * purpose:    Transforms four cosine coefficients back into four individual
 *             luminance values for a 2x2 block.
 * parameters: coeffs: a struct containing the a, b, c, and d coefficients
 *             y1:     pointer to store the top-left luminance
 *             y2:     pointer to store the top-right luminance
 *             y3:     pointer to store the bottom-left luminance
 *             y4:     pointer to store the bottom-right luminance
 * return:     None.
 * expect:     Pointers y1 through y4 must be non-null.
 * notes:      This is the inverse operation of apply_dct.
*/
void inverse_dct(struct dct_coeffs coeffs, 
                 float *y1, float *y2, float *y3, float *y4);

#endif
