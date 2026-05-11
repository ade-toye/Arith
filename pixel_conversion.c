/**************************************************************
 *                     pixel_conversion.c
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


#include "pixel_conversion.h"
#include <math.h>

/*
 * name:       rgb_to_ypbpr
 * purpose:    Converts a single RGB pixel to the YPbPr color space.
 * parameters: rgb:   the struct containing red, green, and blue components
 *             denom: the denominator used to scale the integer RGB values
 * return:     A struct ypbpr containing floating point Y, Pb, and Pr values.
 * expect:     denom is greater than 0.
 * notes:      Scaling is performed to normalize RGB values to the [0, 1] 
 *             range.
*/
struct ypbpr rgb_to_ypbpr(struct Pnm_rgb rgb, unsigned denom) 
{
        float r = (float)rgb.red / denom;
        float g = (float)rgb.green / denom;
        float b = (float)rgb.blue / denom;
        
        // Convert RGB to YPbPr using standard formulas
        struct ypbpr pix;
        pix.y  = 0.299 * r + 0.587 * g + 0.114 * b;
        pix.pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        pix.pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

        return pix;
}

/*
 * name:       ypbpr_to_rgb
 * purpose:    Converts a YPbPr pixel back to the RGB color space.
 * parameters: pix:   the struct containing Y, Pb, and Pr floating point 
 *                    values
 *             denom: the target denominator for the resulting RGB pixel
 * return:     A struct Pnm_rgb scaled by the provided denominator.
 * expect:     None.
 * notes:      Clamps resulting RGB values to the [0, 1] range before scaling.
*/
struct Pnm_rgb ypbpr_to_rgb(struct ypbpr pix, unsigned denom) 
{
        float r = 1.0 * pix.y + 1.402 * pix.pr;
        float g = 1.0 * pix.y - 0.344136 * pix.pb - 0.714136 * pix.pr;
        float b = 1.0 * pix.y + 1.772 * pix.pb;

        // Clamp RGB values to [0, 1] range before scaling
        if (r < 0) r = 0; 
        if (r > 1) r = 1; 
        if (g < 0) g = 0; 
        if (g > 1) g = 1; 
        if (b < 0) b = 0; 
        if (b > 1) b = 1;

        // Scale back to the target denominator and round to nearest integer
        struct Pnm_rgb out;
        out.red   = (unsigned)round(r * denom);
        out.green = (unsigned)round(g * denom);
        out.blue  = (unsigned)round(b * denom);

        return out;
}

/*
 * name:       quantize_small
 * purpose:    Quantizes a floating point DCT coefficient (b, c, or d).
 * parameters: f: the floating point coefficient to quantize
 * return:     An integer representation of the value scaled to a 5-bit range.
 * expect:     None.
 * notes:      Clamps the input to the range [-0.3, 0.3] before quantization.
*/
int quantize_small(float f) 
{
        if (f > 0.3)  f = 0.3; 
        if (f < -0.3) f = -0.3;
        return (int)round(f * (31.0 / 0.3));
}

/*
 * name:       unquantize_small
 * purpose:    Converts a quantized 5-bit integer back to a float.
 * parameters: n: the quantized integer value
 * return:     The floating point representation of the coefficient.
 * expect:     n is within the range allowed by a 5-bit signed integer.
 * notes:      None.
*/
float unquantize_small(int n) 
{ 
        return (float)n * (0.3 / 31.0); 
}