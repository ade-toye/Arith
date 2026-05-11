/**************************************************************
 *                     dct.c
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This file implements the discrete cosine transform (DCT) and 
 *   its inverse for 2x2 blocks of luminance values. The apply_dct 
 *   function computes the DCT coefficients from four Y values, 
 *   while the inverse_dct function reconstructs the Y values 
 *   from the DCT coefficients. 
 *
**************************************************************/

#include "dct.h"
#include "pixel_conversion.h"

/* 
 * name:       apply_dct
 * purpose:    Transforms four luminance (Y) values from a 2x2 block into 
 *             cosine space coefficients (a, b, c, d).
 * parameters: y1: the luminance of the top-left pixel
 *             y2: the luminance of the top-right pixel
 *             y3: the luminance of the bottom-left pixel
 *             y4: the luminance of the bottom-right pixel
 * return:     A struct dct_coeffs containing the four calculated 
 *             coefficients.
 * expect:     Luminance values are typically between 0.0 and 1.0.
 * notes:      Calculations follow the standard 2x2 discrete cosine transform.
*/
struct dct_coeffs apply_dct(float y1, float y2, float y3, float y4) 
{
        struct dct_coeffs coeffs;

        coeffs.a = (y4 + y3 + y2 + y1) / 4.0;
        coeffs.b = (y4 + y3 - y2 - y1) / 4.0;
        coeffs.c = (y4 - y3 + y2 - y1) / 4.0;
        coeffs.d = (y4 - y3 - y2 + y1) / 4.0;

        return coeffs;
}

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
 * notes:      The inverse transform reconstructs the spatial luminance 
 *             values.
*/
void inverse_dct(struct dct_coeffs coeffs, 
                 float *y1, float *y2, float *y3, float *y4) 
{
        float a = coeffs.a;
        float b = coeffs.b;
        float c = coeffs.c;
        float d = coeffs.d;

        // Reconstruct the original Y values using the inverse DCT formula
        *y1 = a - b - c + d;
        *y2 = a - b + c - d;
        *y3 = a + b - c - d;
        *y4 = a + b + c + d;
}