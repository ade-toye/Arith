/**************************************************************
 *                     bitpack.c
 *
 *   Assignment: Homework 4
 *   Authors:    Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
 *   Date:       March 4, 2026
 *
 *   This module provides functions for packing and unpacking unsigned and
 *   signed integers into and from 64-bit words. It includes checks for
 *   fitting values within specified bit widths and handles edge cases to
 *   avoid undefined behavior.
 *
**************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "bitpack.h"
#include "except.h"
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/*
 * name:       safe_shl
 * purpose:    Safely performs a left shift on a 64-bit unsigned integer.
 * parameters: n:     the value to shift
 *             shift: the number of bits to shift
 * return:     The result of (n << shift), or 0 if shift >= 64.
 * expect:     None.
 * notes:      Avoids undefined behavior inherent in shifting 
 *             by the bit-width.
*/
static uint64_t safe_shl(uint64_t n, unsigned shift)
{
        return (shift >= 64) ? 0 : (n << shift);
}

/*
 * name:       safe_shr
 * purpose:    Safely performs a right shift on a 64-bit unsigned integer.
 * parameters: n:     the value to shift
 *             shift: the number of bits to shift
 * return:     The result of (n >> shift), or 0 if shift >= 64.
 * expect:     None.
 * notes:      Avoids undefined behavior inherent in shifting by 
 *             the bit-width.
*/
static uint64_t safe_shr(uint64_t n, unsigned shift)
{
        return (shift >= 64) ? 0 : (n >> shift);
}

/*
 * name:       Bitpack_fitsu
 * purpose:    Determines if an unsigned integer can fit within a given width.
 * parameters: n:     the unsigned value to check
 *             width: the bit-width of the target field
 * return:     True if n fits in width bits, false otherwise.
 * expect:     width is in the range [0, 64].
 * notes:      Width 0 fits only the value 0. Width 64 fits all 
 *             uint64_t values.
*/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);

        if (width == 0) {
                return n == 0;
        }
        if (width == 64) {
                return true;
        }
        return n < safe_shl(1, width);
}

/*
 * name:       Bitpack_fitss
 * purpose:    Determines if a signed integer can fit within a given 
 *             width.
 * parameters: n:     the signed value to check
 *             width: the bit-width of the target field
 * return:     True if n fits in width bits, false otherwise.
 * expect:     width is in the range [0, 64].
 * notes:      Width 0 fits only the value 0. Width 64 fits 
 *             all int64_t values.
*/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= 64);

        if (width == 0) {
                return n == 0;
        }
        if (width == 64) {
                return true;
        }
        int64_t limit = (int64_t)1 << (width - 1);
        return (n >= -limit && n < limit);
}

/*
 * name:       Bitpack_getu
 * purpose:    Extracts an unsigned value from a word.
 * parameters: word:  the 64-bit container
 *             width: the number of bits in the field
 *             lsb:   the index of the field's least significant bit
 * return:     The extracted unsigned value.
 * expect:     width <= 64 and lsb + width <= 64.
 * notes:      Returns 0 if width is 0. If width is 64, lsb must be 0.
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(lsb <= 64);
        assert(lsb + width <= 64);

        if (width == 0) {
                return 0;
        }
        if (width == 64) {
                assert(lsb == 0);
                return word;
        }
        uint64_t mask = safe_shl(1, width) - 1;
        return safe_shr(word, lsb) & mask;
}

/*
 * name:       Bitpack_gets
 * purpose:    Extracts a signed value from a word.
 * parameters: word:  the 64-bit container
 *             width: the number of bits in the field
 *             lsb:   the index of the field's least significant bit
 * return:     The extracted signed value.
 * expect:     width <= 64 and lsb + width <= 64.
 * notes:      Returns 0 if width is 0. Uses sign extension to preserve sign.
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(lsb <= 64);
        assert(lsb + width <= 64);

        if (width == 0) {
                return 0;
        }
        if (width == 64) {
                assert(lsb == 0);
                return (int64_t)word;
        }
        uint64_t val = Bitpack_getu(word, width, lsb);
        unsigned shift = 64 - width;
        return ((int64_t)safe_shl(val, shift)) >> shift;
}

/*
 * name:       Bitpack_newu
 * purpose:    Creates a new word by overwriting a field with an 
 *             unsigned value.
 * parameters: word:  the original 64-bit word
 *             width: the number of bits in the field
 *             lsb:   the index of the field's least significant bit
 *             value: the unsigned value to pack
 * return:     A new 64-bit word with the specified field replaced.
 * expect:     width <= 64 and lsb + width <= 64.
 *             value must fit in width unsigned bits or Bitpack_Overflow 
 *             is raised.
 * notes:      If width is 0, returns word unchanged. If width is 64, 
 *             lsb must be 0.
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        assert(width <= 64);
        assert(lsb <= 64);
        assert(lsb + width <= 64);

        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        if (width == 0) {
                return word;
        }
        if (width == 64) {
                assert(lsb == 0);
                return value;
        }
        uint64_t mask = safe_shl(safe_shl(1, width) - 1, lsb);
        uint64_t cleared = word & ~mask;
        return cleared | safe_shl(value, lsb);
}

/*
 * name:       Bitpack_news
 * purpose:    Creates a new word by overwriting a field with a signed value.
 * parameters: word:  the original 64-bit word
 *             width: the number of bits in the field
 *             lsb:   the index of the field's least significant bit
 *             value: the signed value to pack
 * return:     A new 64-bit word with the specified field replaced.
 * expect:     width <= 64 and lsb + width <= 64.
 *             value must fit in width signed bits or Bitpack_Overflow 
 *             is raised.
 * notes:      If width is 0, returns word unchanged. If width is 64, 
 *             lsb must be 0.
 *             Signed values are packed by masking to the low width bits.
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
        assert(width <= 64);
        assert(lsb <= 64);
        assert(lsb + width <= 64);

        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        if (width == 0) {
                return word;
        }
        if (width == 64) {
                assert(lsb == 0);
                return (uint64_t)value;
        }
        uint64_t mask = safe_shl(1, width) - 1;
        return Bitpack_newu(word, width, lsb, ((uint64_t)value) & mask);
}